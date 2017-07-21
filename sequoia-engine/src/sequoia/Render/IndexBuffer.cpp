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
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/IndexBuffer.h"

namespace sequoia {

namespace render {

static const char* indexTypeToString(IndexBuffer::IndexType type) {
  switch(type) {
  case IndexBuffer::IT_UInt32:
    return "std::uint32_t";
  default:
    sequoia_unreachable("invalid type");
  }
}

static std::size_t sizeOfIndexType(IndexBuffer::IndexType type) {
  switch(type) {
  case IndexBuffer::IT_UInt32:
    return sizeof(std::uint32_t);
  default:
    sequoia_unreachable("invalid type");
  }
}

IndexBuffer::IndexBuffer(BufferKind kind, IndexType type) : Buffer(kind), type_(type) {}

std::size_t IndexBuffer::getSizeOfIndexType() const noexcept { return sizeOfIndexType(type_); }

std::pair<std::string, std::string> IndexBuffer::toStringImpl() const {
  return std::make_pair("IndexBuffer",
                        core::format("%s"
                                     "type = %s,\n",
                                     Base::toStringImpl().second, indexTypeToString(type_)));
}

} // namespace render

} // namespace sequoia
