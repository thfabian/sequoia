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
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/VertexBuffer.h"

namespace sequoia {

namespace render {

VertexBuffer::VertexBuffer(BufferKind kind, const VertexLayout& layout)
    : Buffer(kind), layout_(layout) {}

VertexBuffer::~VertexBuffer() {}

void VertexBuffer::allocateVertices(std::size_t numVertices, Buffer::UsageHint hint) {
  allocate(numVertices * layout_.SizeOf, hint);
}

std::pair<std::string, std::string> VertexBuffer::toStringImpl() const {
  return std::make_pair("VertexBuffer",
                        core::format("{}"
                                     "layout = {},\n",
                                     Base::toStringImpl().second, layout_.toString()));
}

} // namespace render

} // namespace sequoia
