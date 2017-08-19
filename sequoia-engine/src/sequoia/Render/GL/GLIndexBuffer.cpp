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
#include "sequoia/Render/GL/GLIndexBuffer.h"

namespace sequoia {

namespace render {

GLIndexBuffer::GLIndexBuffer(IndexBuffer::IndexType type)
    : IndexBuffer(BK_GLIndexBuffer, type), glBuffer_(GL_ELEMENT_ARRAY_BUFFER, 1) {}

GLIndexBuffer::~GLIndexBuffer() {}

void GLIndexBuffer::write(const void* src, std::size_t offset, std::size_t length,
                          bool discardBuffer) {
  glBuffer_.write(src, offset, length, discardBuffer);
}

void GLIndexBuffer::read(std::size_t offset, std::size_t length, void* dest) {
  glBuffer_.read(offset, length, dest);
}

void GLIndexBuffer::bindForDrawing() { glBuffer_.bind(GLBuffer::BK_Draw); }

void GLIndexBuffer::bindForModify() { glBuffer_.bind(GLBuffer::BK_Modify); }

void GLIndexBuffer::nextTimestep() { glBuffer_.nextTimestep(); }

GLenum GLIndexBuffer::getGLIndexType() const {
  switch(getIndexType()) {
  case IndexBuffer::IT_UInt8:
    return GL_UNSIGNED_BYTE;
  case IndexBuffer::IT_UInt16:
    return GL_UNSIGNED_SHORT;
  case IndexBuffer::IT_UInt32:
    return GL_UNSIGNED_INT;
  default:
    sequoia_unreachable("invalid type");
  }
}

void* GLIndexBuffer::lockImpl(Buffer::LockOption option) { return glBuffer_.lock(option); }

void GLIndexBuffer::unlockImpl() { glBuffer_.unlock(); }

void GLIndexBuffer::allocateImpl(std::size_t numBytes, Buffer::UsageHint usageHint) {
  glBuffer_.allocate(numBytes, usageHint);
}

std::pair<std::string, std::string> GLIndexBuffer::toStringImpl() const {
  return std::make_pair("GLIndexBuffer",
                        core::format("%s"
                                     "glBuffer = %s,\n",
                                     Base::toStringImpl().second, glBuffer_.toString()));
}

} // render

} // namespace sequoia
