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

#include "sequoia-engine/Render/GL/GLVertexBuffer.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"

namespace sequoia {

namespace render {

GLVertexBuffer::GLVertexBuffer(const VertexLayout& layout, int numBuffers)
    : VertexBuffer(BK_GLVertexBuffer, layout), glBuffer_(GL_ARRAY_BUFFER, numBuffers) {}

GLVertexBuffer::~GLVertexBuffer() {}

void GLVertexBuffer::writeImpl(const void* src, std::size_t offset, std::size_t length,
                               bool discardBuffer) {
  glBuffer_.write(src, offset, length, discardBuffer);
}

void GLVertexBuffer::readImpl(std::size_t offset, std::size_t length, void* dest) {
  glBuffer_.read(offset, length, dest);
}

void GLVertexBuffer::bindForDrawing() { glBuffer_.bind(GLBuffer::BK_Draw); }

void GLVertexBuffer::bindForModify() { glBuffer_.bind(GLBuffer::BK_Modify); }

void GLVertexBuffer::nextTimestep() { glBuffer_.nextTimestep(); }

void* GLVertexBuffer::lockImpl(Buffer::LockOption option) { return glBuffer_.lock(option); }

void GLVertexBuffer::unlockImpl() { glBuffer_.unlock(); }

void GLVertexBuffer::allocateImpl(std::size_t numBytes, Buffer::UsageHint usageHint) {
  glBuffer_.allocate(numBytes, usageHint);
}

std::pair<std::string, std::string> GLVertexBuffer::toStringImpl() const {
  return std::make_pair("GLVertexBuffer",
                        core::format("{}"
                                     "glBuffer = {},\n",
                                     Base::toStringImpl().second, glBuffer_.toString()));
}

} // render

} // namespace sequoia
