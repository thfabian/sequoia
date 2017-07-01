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
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLFrameBufferObject.h"

namespace sequoia {

namespace render {

GLFrameBufferObject::~GLFrameBufferObject() {}

GLFrameBufferObject::GLFrameBufferObject() : FrameBufferObject(RK_OpenGL), id_(0), texture_(nullptr) {}

unsigned int GLFrameBufferObject::getID() const { return id_; }

void GLFrameBufferObject::bind()
{
  
}

void GLFrameBufferObject::unbind()
{
  
}

std::string GLFrameBufferObject::toString() const {
  return core::format("GLFrameBufferObject[\n"
                      "  id = %s,\n"
                      "]",
                      id_);
}

void GLFrameBufferObject::init()
{
  
}

} // namespace render

} // namespace sequoia
