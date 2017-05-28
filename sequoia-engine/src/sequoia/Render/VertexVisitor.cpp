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

#include "sequoia/Core/Format.h"
#include "sequoia/Math/Math.h"
#include "sequoia/Render/VertexVisitor.h"
#include <sstream>

namespace sequoia {

namespace render {

void VertexVisitorStringifier::visit(const Vertex3DLayout* layout) {
  Vertex3D* vertices = getVerticesPtr(layout);

  std::stringstream ss;
  for(std::size_t i = 0; i < getNumVertices(); ++i) {
    ss << core::format("Vertex3D["
                       "  Position = %s,\n"
                       "  Normal = %s,\n"
                       "  TexCoord = %s,\n"
                       "  Color = %s\n"
                       "]",
                       math::make_vec3(vertices[i].Position), math::make_vec3(vertices[i].Normal),
                       math::make_vec2(vertices[i].TexCoord), math::make_vec4(vertices[i].Color))
       << ((i == getNumVertices() - 1) ? "," : "") << "\n";
  }

  string_ = ss.str();
}

void VertexVisitorStringifier::visit(const Vertex2DLayout* layout) {
  Vertex2D* vertices = getVerticesPtr(layout);

  std::stringstream ss;
  for(std::size_t i = 0; i < getNumVertices(); ++i) {
    ss << core::format("Vertex2D["
                       "  Position = %s,\n"
                       "  TexCoord = %s,\n"
                       "  Color = %s\n"
                       "]",
                       math::make_vec2(vertices[i].Position), math::make_vec2(vertices[i].TexCoord),
                       math::make_vec4(vertices[i].Color))
       << ((i == getNumVertices() - 1) ? "," : "") << "\n";
  }

  string_ = ss.str();
}

} // namespace render

} // namespace sequoia
