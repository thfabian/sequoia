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

#include "sequoia/Render/VertexVisitor.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Math/Math.h"
#include <sstream>

namespace sequoia {

namespace render {

template <class VertexType>
std::string toStringImpl(const VertexType* vertices, const std::size_t numVertices) {
  std::stringstream ss;
  for(std::size_t i = 0; i < numVertices; ++i)
    ss << VertexType::toString(vertices[i]) << ((i == numVertices - 1) ? "" : ",") << "\n";
  return ss.str();
}

#define SEQUOIA_VERTEXVISITOR_IMPL(r, data, elem)                                                  \
  void VertexVisitorStringifier::visit(const elem* layout) {                                       \
    string_ = toStringImpl(getVerticesPtr(layout), getNumVertices());                              \
  }

BOOST_PP_SEQ_FOR_EACH(SEQUOIA_VERTEXVISITOR_IMPL, _, SEQUOIA_VERTEX_LAYOUT_SEQ)

#undef SEQUOIA_VERTEXVISITOR_IMPL

} // namespace render

} // namespace sequoia
