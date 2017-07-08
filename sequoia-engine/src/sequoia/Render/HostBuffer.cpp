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

#include "sequoia/Core/Byte.h"
#include "sequoia/Core/Memory.h"
#include "sequoia/Render/HostBuffer.h"
#include <cstring>

namespace sequoia {

namespace render {

HostBuffer::HostBuffer() : Buffer(BK_HostBuffer), dataPtr_(nullptr) {}

std::unique_ptr<HostBuffer> HostBuffer::create(std::size_t numBytes) {
  auto buffer = std::make_unique<HostBuffer>();
  buffer->allocate(numBytes, UH_Dynamic);
  return buffer;
}

HostBuffer::~HostBuffer() {
  if(dataPtr_) {
    memory::aligned_free(dataPtr_);
    dataPtr_ = nullptr;
  }
}

void HostBuffer::write(const void* src, std::size_t offset, std::size_t length,
                       bool discardBuffer) {
  (void)discardBuffer;
  SEQUOIA_ASSERT_MSG((offset + length) <= getNumBytes(), "out of bound writing");
  std::memcpy(static_cast<void*>(((Byte*)dataPtr_ + offset)), src, length);
}

void HostBuffer::read(std::size_t offset, std::size_t length, void* dest) {
  SEQUOIA_ASSERT_MSG((offset + length) <= getNumBytes(), "out of bound reading");
  std::memcpy(dest, static_cast<void*>(((Byte*)dataPtr_ + offset)), length);
}

void HostBuffer::allocateImpl(std::size_t numBytes, Buffer::UsageHint usageHint) {
  dataPtr_ = memory::aligned_alloc(numBytes);
}

std::pair<std::string, std::string> HostBuffer::toStringImpl() const {
  return std::make_pair("HostBuffer", Base::toStringImpl().second);
}

} // namespace render

} // namespace sequoia