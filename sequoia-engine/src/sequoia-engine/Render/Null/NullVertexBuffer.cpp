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
#include "sequoia-engine/Render/Null/NullVertexBuffer.h"

namespace sequoia {

namespace render {

NullVertexBuffer::NullVertexBuffer(const VertexLayout2& layout, int numBuffers)
    : VertexBuffer(BK_NullVertexBuffer, layout) {
  buffer_ = std::make_unique<HostBuffer>();
}

NullVertexBuffer::~NullVertexBuffer() {}

void NullVertexBuffer::writeImpl(const void* src, std::size_t offset, std::size_t length,
                                 bool discardBuffer) {
  buffer_->write(src, offset, length, discardBuffer);
}

void NullVertexBuffer::readImpl(std::size_t offset, std::size_t length, void* dest) {
  buffer_->read(offset, length, dest);
}

void* NullVertexBuffer::lockImpl(Buffer::LockOption option) {
  buffer_->lock(option);
  return buffer_->get();
}

void NullVertexBuffer::unlockImpl() { buffer_->unlock(); }

void NullVertexBuffer::allocateImpl(std::size_t numBytes, Buffer::UsageHint usageHint) {
  buffer_->allocate(numBytes, usageHint);
}

std::pair<std::string, std::string> NullVertexBuffer::toStringImpl() const {
  return std::make_pair("NullVertexBuffer",
                        core::format("{}"
                                     "{}\n",
                                     Base::toStringImpl().second, buffer_->toString()));
}

} // render

} // namespace sequoia
