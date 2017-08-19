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
#include "sequoia/Core/Memory.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/VertexData.h"
#include "sequoia/Render/VertexVisitor.h"
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

void VertexData::accept(Buffer::LockOption option, VertexVisitor& visitor) const {
  BufferGuard guard(getVertexBuffer(), option);
  visitor.setVerticesPtr(guard.get());
  visitor.setNumVertices(getVertexBuffer()->getNumVertices());
  getVertexBuffer()->getLayout()->accept(visitor);
}

void VertexData::dump() const {
  std::cout << "Vertices = [\n";

  // Print vertices
  VertexVisitorStringifier visitor;
  accept(Buffer::LO_ReadOnly, visitor);
  std::cout << "  " << core::indent(visitor.toString()) << "]";

  // Print indices
  if(hasIndices()) {
    std::cout << ",\nIndices = [\n";

    BufferGuard guard(getIndexBuffer(), Buffer::LO_ReadOnly);
    Byte* data = static_cast<Byte*>(guard.get());
    
    auto type = getIndexBuffer()->getIndexType();
    for(std::size_t i = 0; i < getNumIndices(); i++, data += getIndexBuffer()->getSizeOfIndexType())
      std::cout << "  " << indexToString(data, type) << ((i == getNumIndices() - 1) ? "" : ",")
                << "\n";
    
    std::cout << "]\n";
  } else
    std::cout << "\n";
}

std::string VertexData::toString() const {
  auto stringPair = toStringImpl();
  return core::format("%s[\n  %s]", stringPair.first, core::indent(stringPair.second));
}

std::pair<std::string, std::string> VertexData::toStringImpl() const {
  return std::make_pair("VertexData",
                        core::format("drawMode = %s,\n", drawModeToString(drawMode_)));
}

} // namespace render

} // namespace sequoia
