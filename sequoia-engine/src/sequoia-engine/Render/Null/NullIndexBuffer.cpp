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

#include "sequoia-engine/Render/Null/NullIndexBuffer.h"
#include "sequoia-engine/Core/Format.h"

namespace sequoia {

namespace render {

NullIndexBuffer::NullIndexBuffer(IndexBuffer::IndexType type)
    : IndexBuffer(BK_NullIndexBuffer, type) {
  buffer_ = std::make_unique<core::HostBuffer>();
}

NullIndexBuffer::~NullIndexBuffer() {}

void NullIndexBuffer::writeImpl(const void* src, std::size_t offset, std::size_t length,
                                bool discardBuffer) {
  buffer_->write(src, offset, length, discardBuffer);
}

void NullIndexBuffer::readImpl(std::size_t offset, std::size_t length, void* dest) {
  buffer_->read(offset, length, dest);
}

void* NullIndexBuffer::lockImpl(Buffer::LockOption option) {
  buffer_->lock(option);
  return buffer_->get();
}

void NullIndexBuffer::unlockImpl() { buffer_->unlock(); }

void NullIndexBuffer::allocateImpl(std::size_t numBytes, Buffer::UsageHint usageHint) {
  buffer_->allocate(numBytes, usageHint);
}

std::pair<std::string, std::string> NullIndexBuffer::toStringImpl() const {
  return std::make_pair("NullIndexBuffer",
                        core::format("{}"
                                     "buffer = {}\n",
                                     Base::toStringImpl().second, buffer_->toString()));
}

} // render

} // namespace sequoia
