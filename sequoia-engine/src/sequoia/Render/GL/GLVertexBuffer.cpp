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
#include "sequoia/Render/GL/GLVertexBuffer.h"

namespace sequoia {

namespace render {

GLVertexBuffer::GLVertexBuffer(const VertexLayout* layout, int numBuffers)
    : VertexBuffer(BK_GLVertexBuffer, layout), glBuffer_(GL_ARRAY_BUFFER, numBuffers) {}

GLVertexBuffer::~GLVertexBuffer() {}

void GLVertexBuffer::write(const void* src, std::size_t offset, std::size_t length,
                           bool discardBuffer) {
  glBuffer_.write(src, offset, length, discardBuffer);
}

void GLVertexBuffer::read(std::size_t offset, std::size_t length, void* dest) {
  glBuffer_.read(offset, length, dest);
}

void GLVertexBuffer::bindForDrawing() { glBuffer_.bind(GLBuffer::BK_Draw); }

void GLVertexBuffer::nextTimestep() { glBuffer_.nextTimestep(); }

void* GLVertexBuffer::lockImpl(Buffer::LockOption option) { return glBuffer_.lock(option); }

void GLVertexBuffer::unlockImpl() { glBuffer_.unlock(); }

void GLVertexBuffer::allocateImpl(std::size_t numBytes, Buffer::UsageHint usageHint) {
  glBuffer_.allocate(numBytes, usageHint);
}

std::pair<std::string, std::string> GLVertexBuffer::toStringImpl() const {
  return std::make_pair("GLVertexBuffer", core::format("%s"
                                                       "glBuffer = %s,\n",
                                                       Base::toStringImpl().second,
                                                       core::indent(glBuffer_.toString())));
}

} // render

} // namespace sequoia
