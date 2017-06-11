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
#include "sequoia/Math/Math.h"
#include "sequoia/Render/VertexVisitor.h"
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

void VertexVisitorStringifier::visit(const Vertex3DLayout* layout) {
  string_ = toStringImpl(getVerticesPtr(layout), getNumVertices());
}

void VertexVisitorStringifier::visit(const Vertex2DLayout* layout) {
  string_ = toStringImpl(getVerticesPtr(layout), getNumVertices());
}

} // namespace render

} // namespace sequoia
