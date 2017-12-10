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

#include "sequoia-engine/Core/Byte.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Memory.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/VertexAdapter.h"
#include "sequoia-engine/Render/VertexData.h"
#include <iostream>

namespace sequoia {

namespace render {

static std::string indexToString(void* data, IndexBuffer::IndexType type) {
  switch(type) {
  case IndexBuffer::IT_UInt8:
    return std::to_string(*static_cast<std::uint8_t*>(data));
  case IndexBuffer::IT_UInt16:
    return std::to_string(*static_cast<std::uint16_t*>(data));
  case IndexBuffer::IT_UInt32:
    return std::to_string(*static_cast<std::uint32_t*>(data));
  default:
    sequoia_unreachable("invalid IndexType");
  }
}

static const char* drawModeToString(VertexData::DrawModeKind mode) {
  switch(mode) {
  case VertexData::DrawModeKind::DM_Triangles:
    return "Triangles";
  default:
    sequoia_unreachable("invalid DrawModeKind");
  }
}

VertexData::VertexData(RenderSystemKind renderSystemKind, VertexData::DrawModeKind drawMode)
    : RenderSystemObject(renderSystemKind), bbox_(nullptr), drawMode_(drawMode) {}

VertexData::~VertexData() {}

void VertexData::dump() const {

  // Print vertices
  {
    std::cout << "Vertices = [\n";

    BufferGuard guard(getVertexBuffer(), Buffer::LO_ReadOnly);
    Byte* data = static_cast<Byte*>(guard.get());

    const std::size_t numVertices = getNumVertices();
    const VertexLayout& layout = getLayout();
    VertexAdapter adapter(layout);

    for(std::size_t i = 0; i < numVertices; ++i, data += layout.SizeOf) {
      adapter.copyFrom(data);
      std::cout << "  " << adapter << ((i == numVertices - 1) ? "" : ",") << "\n";
    }

    std::cout << "]\n";
  }

  // Print indices
  if(hasIndices()) {
    std::cout << ",\nIndices = [\n";

    BufferGuard guard(getIndexBuffer(), Buffer::LO_ReadOnly);
    Byte* data = static_cast<Byte*>(guard.get());

    const std::size_t numIndices = getNumIndices();
    auto type = getIndexBuffer()->getIndexType();

    for(std::size_t i = 0; i < numIndices; ++i, data += getIndexBuffer()->getSizeOfIndexType())
      std::cout << "  " << indexToString(data, type) << ((i == numIndices - 1) ? "" : ",") << "\n";

    std::cout << "]\n";
  } else
    std::cout << "\n";
}

std::string VertexData::toString() const {
  auto stringPair = toStringImpl();
  return core::format("{}[\n  {}]", stringPair.first, core::indent(stringPair.second));
}

std::pair<std::string, std::string> VertexData::toStringImpl() const {
  return std::make_pair("VertexData",
                        core::format("drawMode = {},\n", drawModeToString(drawMode_)));
}

} // namespace render

} // namespace sequoia
