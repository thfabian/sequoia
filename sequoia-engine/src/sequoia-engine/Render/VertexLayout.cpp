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
#include "sequoia-engine/Render/VertexLayout.h"
#include <memory>
#include <sstream>
#include <utility>

namespace sequoia {

namespace render {

static const char* typeToString(VertexLayout2::TypeID typeID) {
  switch(typeID) {
  case VertexLayout2::Invalid:
    return "Invalid";
  case VertexLayout2::UInt8:
    return "Uint8";
  case VertexLayout2::Float32:
    return "Float32";
  default:
    sequoia_unreachable("invalid type");
  }
}

std::string VertexLayout2::toString() const {
  std::stringstream ss;
  ss << "VertexLayout[\n";

  auto attributeToString = [&ss](const char* name, const Attribute& attrib) {
    ss << "  " << name << " = Attribute[\n";
    ss << "    Type = " << typeToString(attrib.Type) << ",\n";
    ss << "    Offset = " << attrib.Offset << ",\n";
    ss << "    NumElements = " << attrib.NumElements << ",\n";
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
