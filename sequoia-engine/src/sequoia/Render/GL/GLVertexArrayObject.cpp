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

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"

namespace sequoia {

namespace render {

GLVertexArrayObject::~GLVertexArrayObject() {}

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

  bind();

  // Set the vertex attributes
  
  // -> use the attributes in GLAttributes ..
  
  unbind();
}

} // namespace render

} // namespace sequoia
