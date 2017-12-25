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
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/Vertex.h"
#include "sequoia-engine/Render/VertexLayout.h"
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <memory>
#include <sstream>
#include <utility>

namespace sequoia {

namespace render {

static const char* typeToString(VertexLayout::TypeID typeID) {
  switch(typeID) {
  case VertexLayout::Invalid:
    return "Invalid";
#define VERTEX_LAYOUT_TYPE(Type, Name)                                                             \
  case VertexLayout::Name:                                                                         \
    return BOOST_PP_STRINGIZE(Name);
#include "sequoia-engine/Render/VertexLayout.inc"
#undef VERTEX_LAYOUT_TYPE
  default:
    sequoia_unreachable("invalid type");
  }
}

#define SEQUOIA_PP_VL_GET_NAME(r, Data, Vertex)                                                    \
  case VertexID::Vertex:                                                                           \
    return BOOST_PP_STRINGIZE(Vertex);

const char* VertexLayout::getName() const noexcept {
  auto id = static_cast<VertexID>(ID);
  switch(id) {
    BOOST_PP_SEQ_FOR_EACH(SEQUOIA_PP_VL_GET_NAME, Data, SEQUOIA_VERTICES)
  default:
    sequoia_unreachable("invalid vertex ID");
  }
}

std::string VertexLayout::toString() const {
  std::stringstream ss;
  ss << "VertexLayout[\n";
  ss << "  ID = " << getName() << "\n";

  auto attributeToString = [&ss](const char* name, const Attribute& attrib) {
    ss << "  " << name << " = Attribute[\n";
    ss << "    Type = " << typeToString(attrib.Type) << ",\n";
    ss << "    Offset = " << int(attrib.Offset) << ",\n";
    ss << "    NumElements = " << int(attrib.NumElements) << ",\n";
    ss << "    Normalized = " << attrib.Normalize << "\n";
    ss << "  ]\n";
  };

  if(hasPosition())
    attributeToString("Position", Position);

  if(hasNormal())
    attributeToString("Normal", Normal);

  if(hasTexCoord())
    attributeToString("TexCoord", TexCoord);

  if(hasColor())
    attributeToString("Color", Color);

  ss << "]";
  return ss.str();
}

} // namespace render

} // namespace sequoia
