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

#include "sequoia-engine/Render/D3D12/D3D12IndexBuffer.h"
#include "sequoia-engine/Core/Format.h"

namespace sequoia {

namespace render {

D3D12IndexBuffer::D3D12IndexBuffer(IndexBuffer::IndexType type)
    : IndexBuffer(BK_D3D12IndexBuffer, type) {
  buffer_ = std::make_unique<core::HostBuffer>();
}

D3D12IndexBuffer::~D3D12IndexBuffer() {}

void D3D12IndexBuffer::writeImpl(const void* src, std::size_t offset, std::size_t length,
                                bool discardBuffer) {
  buffer_->write(src, offset, length, discardBuffer);
}

void D3D12IndexBuffer::readImpl(std::size_t offset, std::size_t length, void* dest) {
  buffer_->read(offset, length, dest);
}

void* D3D12IndexBuffer::lockImpl(Buffer::LockOption option) {
  buffer_->lock(option);
  return buffer_->get();
}

void D3D12IndexBuffer::unlockImpl() { buffer_->unlock(); }

void D3D12IndexBuffer::allocateImpl(std::size_t numBytes, Buffer::UsageHint usageHint) {
  buffer_->allocate(numBytes, usageHint);
}

std::pair<std::string, std::string> D3D12IndexBuffer::toStringImpl() const {
  return std::make_pair("D3D12IndexBuffer",
                        core::format("{}"
                                     "buffer = {}\n",
                                     Base::toStringImpl().second, buffer_->toString()));
}

} // render

} // namespace sequoia
