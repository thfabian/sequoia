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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Render/GL/GLVertexData.h"
#include "sequoia/Render/HostBuffer.h"

namespace sequoia {

namespace render {

static GLenum getGLType(VertexLayout::Type type) {
  switch(type) {
  case VertexLayout::Float:
    return GL_FLOAT;
  case VertexLayout::UnsignedByte:
    return GL_UNSIGNED_BYTE;
  default:
    sequoia_unreachable("invlid type");
    return GL_INVALID_ENUM;
  }
}

static GLenum getGLDrawMode(VertexData::DrawModeKind mode) {
  switch(mode) {
  case VertexData::DrawModeKind::DM_Triangles:
    return GL_TRIANGLES;
  default:
    sequoia_unreachable("invalid DrawModeKind");
    return GL_INVALID_ENUM;
  }
}

GLVertexData::GLVertexData(const VertexDataParameter& param)
    : VertexData(RK_OpenGL, param.DrawMode), vertexBuffer_(nullptr), indexBuffer_(nullptr),
      vaoID_(0) {
  
  SEQUOIA_ASSERT_MSG(param.NumVertexBuffers <= 1 || param.UseVertexShadowBuffer,
                     "multiple vertex buffers require shadow buffering");
  
  const VertexLayout* layout = param.Layout;

  // Generate VAO, VBO and VEO
  glGenVertexArrays(1, &vaoID_);
  vertexBuffer_ = std::make_unique<GLVertexBuffer>(layout, param.NumVertexBuffers);
  if(param.NumIndices > 0)
    indexBuffer_ = std::make_unique<GLIndexBuffer>(param.IndexType);

  getGLRenderSystem().getStateCacheManager()->bindVertexDataForModify(this);

  // Enable the active attributes
  if(layout->hasPosition()) {
    glEnableVertexAttribArray(GLVertexAttribute::Position);
    glVertexAttribPointer(GLVertexAttribute::Position, layout->PositionNumElement,
                          getGLType(layout->PositionType), layout->PositionNormalized,
                          layout->SizeOf, (void*)layout->PositionOffset);
  }

  if(layout->hasNormal()) {
    glEnableVertexAttribArray(GLVertexAttribute::Normal);
    glVertexAttribPointer(GLVertexAttribute::Normal, layout->NormalNumElement,
                          getGLType(layout->NormalType), layout->NormalNormalized, layout->SizeOf,
                          (void*)layout->NormalOffset);
  }

  if(layout->hasTexCoord()) {
    glEnableVertexAttribArray(GLVertexAttribute::TexCoord);
    glVertexAttribPointer(GLVertexAttribute::TexCoord, layout->TexCoordNumElement,
                          getGLType(layout->TexCoordType), layout->TexCoordNormalized,
                          layout->SizeOf, (void*)layout->TexCoordOffset);
  }

  if(layout->hasColor()) {
    glEnableVertexAttribArray(GLVertexAttribute::Color);
    glVertexAttribPointer(GLVertexAttribute::Color, layout->ColorNumElement,
                          getGLType(layout->ColorType), layout->ColorNormalized, layout->SizeOf,
                          (void*)layout->ColorOffset);
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
                        core::format("%s"
                                     "vertexBuffer = %s,\n"
                                     "indexBuffer = %s,\n"
                                     "vaoID = %s\n",
                                     Base::toStringImpl().second, vertexBuffer_->toString(),
                                     indexBuffer_ ? indexBuffer_->toString() : "null", vaoID_));
}

} // namespace render

} // namespace sequoia
