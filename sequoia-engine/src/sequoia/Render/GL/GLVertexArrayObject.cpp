//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Render/VertexData.h"

namespace sequoia {

namespace render {

namespace {

static GLenum getGLType(VertexLayout::Type type) {
  switch(type) {
  case VertexLayout::Float:
    return GL_FLOAT;
  case VertexLayout::UnsignedByte:
    return GL_UNSIGNED_BYTE;
  default:
    sequoia_unreachable("invlid Type");
    return GL_INVALID_ENUM;
  }
}

static GLenum getGLUsage(VertexArrayObject::BufferUsageKind usage) {
  switch(usage) {
  case VertexArrayObject::BK_Static:
  case VertexArrayObject::BK_StaticWriteOnly:
    return GL_STATIC_DRAW;
  case VertexArrayObject::BK_Dynamic:
  case VertexArrayObject::BK_DynamicWriteOnly:
    return GL_DYNAMIC_DRAW;
  case VertexArrayObject::BK_DynamicWriteOnlyDiscardable:
    return GL_STREAM_DRAW;
  default:
    return GL_DYNAMIC_DRAW;
  }
}

} // anonymous namespace

GLVertexArrayObject::~GLVertexArrayObject() {
  if(allocated_)
    freeVertexDataDevice();
}

GLVertexArrayObject::GLVertexArrayObject()
    : VertexArrayObject(RenderSystemKind::RK_OpenGL), vaoID_(0), eboID_(0), vboID_(0) {}

void GLVertexArrayObject::bind() {
  glBindVertexArray(vaoID_);
  glBindBuffer(GL_ARRAY_BUFFER, vboID_);

  if(hasIndices())
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID_);
  else
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int GLVertexArrayObject::getVAOID() const { return vaoID_; }

void GLVertexArrayObject::writeVertexData(std::size_t offset, std::size_t length) {
  
  // TODO: use state-cache manager  
  bind();

  // TODO: Discardable data should use glBufferData(GL_ARRAY_BUFFER, ..., NULL, ...) first

  if(offset == 0) {
    glBufferData(GL_ARRAY_BUFFER, getNumVertexBytes(length), data_->getVerticesPtr(),
                 getGLUsage(usage_));
  } else {
    glBufferSubData(GL_ARRAY_BUFFER, offset, getNumVertexBytes(length), data_->getVerticesPtr());
  }

  // TODO: Frequently updated data should use glMapBuffer
}

void GLVertexArrayObject::writeIndexData(std::size_t offset, std::size_t length) {
  if(!data_->hasIndices())
    return;

  // TODO: use state-cache manager
  bind();

  // TODO: Discardable data should use glBufferData(GL_ARRAY_BUFFER, ..., NULL, ...) first

  if(offset == 0) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getNumIndexBytes(length), data_->getIndicesPtr(),
                 getGLUsage(usage_));
  } else {
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, getNumIndexBytes(length),
                    data_->getIndicesPtr());
  }

  // TODO: Frequently updated data should use glMapBuffer
}

void GLVertexArrayObject::attachVertexDataDevice() {

  // Generate the buffers
  glGenVertexArrays(1, &vaoID_);
  glGenBuffers(1, &vboID_);

  if(hasIndices())
    glGenBuffers(1, &eboID_);

  // Set the vertex attributes
  bind();

  const VertexLayout* layout = data_->getLayout();

  if(layout->hasPosition()) {
    glEnableVertexAttribArray(GLVertexAttribute::Position);
    glVertexAttribPointer(GLVertexAttribute::Position, layout->PositionNumElement,
                          getGLType(layout->PositionType), layout->PositionNormalized, 0,
                          (void*)layout->PositionOffset);
  }

  if(layout->hasNormal()) {
    glEnableVertexAttribArray(GLVertexAttribute::Normal);
    glVertexAttribPointer(GLVertexAttribute::Normal, layout->NormalNumElement,
                          getGLType(layout->NormalType), layout->NormalNormalized, 0,
                          (void*)layout->NormalOffset);
  }

  if(layout->hasTexCoord()) {
    glEnableVertexAttribArray(GLVertexAttribute::TexCoord);
    glVertexAttribPointer(GLVertexAttribute::TexCoord, layout->TexCoordNumElement,
                          getGLType(layout->TexCoordType), layout->TexCoordNormalized, 0,
                          (void*)layout->TexCoordOffset);
  }

  if(layout->hasColor()) {
    glEnableVertexAttribArray(GLVertexAttribute::Color);
    glVertexAttribPointer(GLVertexAttribute::Color, layout->ColorNumElement,
                          getGLType(layout->ColorType), layout->ColorNormalized, 0,
                          (void*)layout->ColorOffset);
  }

  // Allocate device memory
  glBufferData(GL_ARRAY_BUFFER, getNumVertexBytes(getNumVertices()), nullptr, getGLUsage(usage_));

  if(hasIndices())
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getNumIndexBytes(getNumIndices()), nullptr,
                 getGLUsage(usage_));

  allocated_ = true;
}

void GLVertexArrayObject::freeVertexDataDevice() {
  glDeleteBuffers(1, &vboID_);

  if(hasIndices())
    glDeleteBuffers(1, &eboID_);

  glDeleteVertexArrays(1, &vaoID_);

  vaoID_ = vboID_ = eboID_ = 0;
  allocated_ = false;
}

std::string GLVertexArrayObject::toString() const {
  return core::format("GLVertexArrayObject[\n"
                      "  vaoID = %i,\n"
                      "  eboID = %i,\n"
                      "  vboID = %i\n"
                      "]",
                      vaoID_, eboID_, vboID_);
}

std::size_t GLVertexArrayObject::getNumVertexBytes(std::size_t length) const {
  return length * data_->getLayout()->SizeOf;
}

std::size_t GLVertexArrayObject::getNumIndexBytes(std::size_t length) const {
  return length * sizeof(VertexIndexType);
}

} // namespace render

} // namespace sequoia
