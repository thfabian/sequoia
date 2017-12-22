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

#include "sequoia-engine/Render/GL/GLIndexBuffer.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"

namespace sequoia {

namespace render {

GLIndexBuffer::GLIndexBuffer(IndexBuffer::IndexType type)
    : IndexBuffer(BK_GLIndexBuffer, type), glBuffer_(GL_ELEMENT_ARRAY_BUFFER) {}

GLIndexBuffer::~GLIndexBuffer() {}

void GLIndexBuffer::writeImpl(const void* src, std::size_t offset, std::size_t length,
                              bool discardBuffer) {
  glBuffer_.write(src, offset, length, discardBuffer);
}

void GLIndexBuffer::readImpl(std::size_t offset, std::size_t length, void* dest) {
  glBuffer_.read(offset, length, dest);
}

void GLIndexBuffer::bind() { glBuffer_.bind(); }

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
                        core::format("{}"
                                     "glBuffer = {},\n",
                                     Base::toStringImpl().second, glBuffer_.toString()));
}

} // render

} // namespace sequoia
