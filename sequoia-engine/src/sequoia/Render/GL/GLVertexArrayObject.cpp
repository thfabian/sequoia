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

#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"

namespace sequoia {

namespace render {

namespace {

static GLenum getGLType(VertexLayout::Type type) {
  switch(type) {
  case VertexLayout::Float:
    return GLenum::GL_FLOAT;
  case VertexLayout::UnsignedByte:
    return GLenum::GL_UNSIGNED_BYTE;
  default:
    sequoia_unreachable("invlid Type");
    return GL_INVALID_ENUM;
  }
}

} // anonymous namespace

GLVertexArrayObject::~GLVertexArrayObject() {
  if(allocated_)
    freeVertexDataDevice();
}

GLVertexArrayObject::GLVertexArrayObject()
    : VertexArrayObject(RenderSystemKind::RK_OpenGL), vaoID_(0), eboID_(0), vboID_(0),
      hasIndices_(false) {}

void GLVertexArrayObject::bind() {
  glBindVertexArray(vaoID_);
  glBindBuffer(GL_ARRAY_BUFFER, vboID_);

  if(hasIndices_)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID_);
  else
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLVertexArrayObject::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void GLVertexArrayObject::updateDevice() {}

bool GLVertexArrayObject::hasIndices() const { return hasIndices_; }

void GLVertexArrayObject::attachVertexDataDevice() {

  // Generate the buffers
  glGenVertexArrays(1, &vaoID_);
  glGenBuffers(1, &vboID_);

  if(hasIndices_)
    glGenBuffers(1, &eboID_);

  // Set the vertex attributes
  bind();

  if(layout_->hasPosition()) {
    glEnableVertexAttribArray(GLVertexAttribute::Position);
    glVertexAttribPointer(GLVertexAttribute::Position, layout_->PositionNumElement,
                          getGLType(layout_->PositionType), false, layout_->SizeOf,
                          (void*)&layout_->PositionOffset);
  }

  if(layout_->hasNormal()) {
    glEnableVertexAttribArray(GLVertexAttribute::Normal);
    glVertexAttribPointer(GLVertexAttribute::Normal, layout_->NormalNumElement,
                          getGLType(layout_->NormalType), false, layout_->SizeOf,
                          (void*)&layout_->NormalOffset);
  }

  if(layout_->hasTexCoord()) {
    glEnableVertexAttribArray(GLVertexAttribute::TexCoord);
    glVertexAttribPointer(GLVertexAttribute::TexCoord, layout_->TexCoordNumElement,
                          getGLType(layout_->TexCoordType), false, layout_->SizeOf,
                          (void*)&layout_->TexCoordOffset);
  }

  if(layout_->hasColor()) {
    glEnableVertexAttribArray(GLVertexAttribute::Color);
    glVertexAttribPointer(GLVertexAttribute::Color, layout_->ColorNumElement,
                          getGLType(layout_->ColorType), true, layout_->SizeOf,
                          (void*)&layout_->ColorOffset);
  }

  unbind();

  allocated_ = true;
}

void GLVertexArrayObject::freeVertexDataDevice() {
  glDeleteBuffers(1, &vboID_);
  if(hasIndices_)
    glDeleteBuffers(1, &eboID_);

  glDeleteVertexArrays(1, &vaoID_);

  vaoID_ = vboID_ = eboID_ = 0;
  allocated_ = false;
}

} // namespace render

} // namespace sequoia
