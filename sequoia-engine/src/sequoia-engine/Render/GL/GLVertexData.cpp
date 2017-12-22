//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Render/GL/GLVertexAttribute.h"
#include "sequoia-engine/Render/GL/GLVertexData.h"
#include "sequoia-engine/Render/HostBuffer.h"

// TODO: convert everything to DSA

namespace sequoia {

namespace render {

namespace {

template <VertexLayout::TypeID ID>
struct InvalidTypeIDToGLEnum {
  static constexpr GLenum value = GL_INVALID_ENUM;
};

template <VertexLayout::TypeID ID>
struct TypeIDToGLEnum {
  static constexpr GLenum value = InvalidTypeIDToGLEnum<ID>::value;
  static_assert(value != GL_INVALID_ENUM, "invalid VertexLayout::TypeID");
};

template <>
struct TypeIDToGLEnum<VertexLayout::UInt8> {
  static constexpr GLenum value = GL_UNSIGNED_BYTE;
};

template <>
struct TypeIDToGLEnum<VertexLayout::Float32> {
  static constexpr GLenum value = GL_FLOAT;
};

} // anonymous namespace

static GLenum getGLDrawMode(VertexData::DrawModeKind mode) {
  switch(mode) {
  case VertexData::DrawModeKind::DM_Triangles:
    return GL_TRIANGLES;
  default:
    sequoia_unreachable("invalid DrawModeKind");
    return GL_INVALID_ENUM;
  }
}

GLenum GLVertexData::getGLType(VertexLayout::TypeID type) {
  switch(type) {
#define VERTEX_LAYOUT_TYPE(Type, Name)                                                             \
  case VertexLayout::Name:                                                                         \
    return TypeIDToGLEnum<VertexLayout::Name>::value;
#include "sequoia-engine/Render/VertexLayout.inc"
#undef VERTEX_LAYOUT_TYPE
  default:
    sequoia_unreachable("invalid type");
    return GL_INVALID_ENUM;
  }
}

GLVertexData::GLVertexData(const VertexDataParameter& param)
    : VertexData(RK_OpenGL, param.DrawMode), vertexBuffer_(nullptr), indexBuffer_(nullptr),
      vaoID_(0) {

  SEQUOIA_ASSERT_MSG(param.NumVertexBuffers <= 1 || param.UseVertexShadowBuffer,
                     "multiple vertex buffers require shadow buffering");

  const VertexLayout& layout = param.Layout;

  // Generate VAO, VBO and VEO
  glGenVertexArrays(1, &vaoID_);
  vertexBuffer_ = std::make_unique<GLVertexBuffer>(layout, param.NumVertexBuffers);
  if(param.NumIndices > 0)
    indexBuffer_ = std::make_unique<GLIndexBuffer>(param.IndexType);

  bindForModify();

  // Enable the active attributes
  if(layout.hasPosition()) {
    glEnableVertexAttribArray(GLVertexAttribute::Position);
    glVertexAttribPointer(GLVertexAttribute::Position, layout.Position.NumElements,
                          getGLType(layout.Position.Type), layout.Position.Normalize, layout.SizeOf,
                          reinterpret_cast<void*>(layout.Position.Offset));
  }

  if(layout.hasNormal()) {
    glEnableVertexAttribArray(GLVertexAttribute::Normal);
    glVertexAttribPointer(GLVertexAttribute::Normal, layout.Normal.NumElements,
                          getGLType(layout.Normal.Type), layout.Normal.Normalize, layout.SizeOf,
                          reinterpret_cast<void*>(layout.Normal.Offset));
  }

  if(layout.hasTexCoord()) {
    glEnableVertexAttribArray(GLVertexAttribute::TexCoord);
    glVertexAttribPointer(GLVertexAttribute::TexCoord, layout.TexCoord.NumElements,
                          getGLType(layout.TexCoord.Type), layout.TexCoord.Normalize, layout.SizeOf,
                          reinterpret_cast<void*>(layout.TexCoord.Offset));
  }

  if(layout.hasColor()) {
    glEnableVertexAttribArray(GLVertexAttribute::Color);
    glVertexAttribPointer(GLVertexAttribute::Color, layout.Color.NumElements,
                          getGLType(layout.Color.Type), layout.Color.Normalize, layout.SizeOf,
                          reinterpret_cast<void*>(layout.Color.Offset));
  }

  // Allocate buffers
  vertexBuffer_->allocateVertices(param.NumVertices, param.VertexBufferUsageHint);
  if(indexBuffer_)
    indexBuffer_->allocateIndices(param.NumIndices);

  // Allocate shadow buffers
  if(param.UseVertexShadowBuffer)
    vertexBuffer_->setShadowBuffer(HostBuffer::create(vertexBuffer_->getNumBytes()));

  if(indexBuffer_ && param.UseIndexShadowBuffer)
    indexBuffer_->setShadowBuffer(HostBuffer::create(indexBuffer_->getNumBytes()));
}

GLVertexData::~GLVertexData() {
  vertexBuffer_.reset();
  indexBuffer_.reset();
  glDeleteVertexArrays(1, &vaoID_);
}

void GLVertexData::bindForDrawing() {
  glBindVertexArray(vaoID_);
  vertexBuffer_->bindForDrawing();
  if(indexBuffer_)
    indexBuffer_->bindForDrawing();
  else
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLVertexData::bindForModify() {
  glBindVertexArray(vaoID_);
  vertexBuffer_->bindForModify();
  if(indexBuffer_)
    indexBuffer_->bindForModify();
  else
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLVertexData::unbind() {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLVertexData::nextTimestep() {
  vertexBuffer_->nextTimestep();
  indexBuffer_->nextTimestep();
}

void GLVertexData::draw() const noexcept {
  if(indexBuffer_) {
    glDrawElements(getGLDrawMode(getDrawMode()), indexBuffer_->getNumIndices(),
                   indexBuffer_->getGLIndexType(), (void*)0);
  } else {
    glDrawArrays(getGLDrawMode(getDrawMode()), 0, vertexBuffer_->getNumVertices());
  }
}

std::pair<std::string, std::string> GLVertexData::toStringImpl() const {
  return std::make_pair("GLVertexData",
                        core::format("{}"
                                     "vertexBuffer = {},\n"
                                     "indexBuffer = {},\n"
                                     "vaoID = {}\n",
                                     Base::toStringImpl().second, vertexBuffer_->toString(),
                                     indexBuffer_ ? indexBuffer_->toString() : "null", vaoID_));
}

} // namespace render

} // namespace sequoia
