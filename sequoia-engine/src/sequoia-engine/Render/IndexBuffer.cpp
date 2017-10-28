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

#include "sequoia-engine/Render/IndexBuffer.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Unreachable.h"

namespace sequoia {

namespace render {

static const char* indexTypeToString(IndexBuffer::IndexType type) {
  switch(type) {
  case IndexBuffer::IT_UInt8:
    return "std::uint8_t";
  case IndexBuffer::IT_UInt16:
    return "std::uint16_t";
  case IndexBuffer::IT_UInt32:
    return "std::uint32_t";
  default:
    sequoia_unreachable("invalid type");
  }
}

static std::size_t sizeOfIndexType(IndexBuffer::IndexType type) {
  switch(type) {
  case IndexBuffer::IT_UInt8:
    return sizeof(std::uint8_t);
  case IndexBuffer::IT_UInt16:
    return sizeof(std::uint16_t);
  case IndexBuffer::IT_UInt32:
    return sizeof(std::uint32_t);
  default:
    sequoia_unreachable("invalid type");
  }
}

IndexBuffer::IndexBuffer(BufferKind kind, IndexType type) : Buffer(kind), type_(type) {}

IndexBuffer::~IndexBuffer() {}

void IndexBuffer::allocateIndices(std::size_t numIndices) {
  allocate(numIndices * getSizeOfIndexType(), Buffer::UH_StaticWriteOnly);
}

std::size_t IndexBuffer::getSizeOfIndexType() const noexcept { return sizeOfIndexType(type_); }

std::pair<std::string, std::string> IndexBuffer::toStringImpl() const {
  return std::make_pair("IndexBuffer",
                        core::format("{}"
                                     "type = {},\n",
                                     Base::toStringImpl().second, indexTypeToString(type_)));
}

} // namespace render

} // namespace sequoia
