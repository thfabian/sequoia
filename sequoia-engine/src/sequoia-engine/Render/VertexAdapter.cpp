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
#include "sequoia-engine/Render/VertexAdapter.h"
#include <sstream>

namespace sequoia {

namespace render {

std::string VertexAdapter::toString() const {
  std::stringstream ss;

  ss << "VertexAdapter[\n";
  auto attributeToString = [&ss](const char* name, const VertexLayout2::Attribute& attrib,
                                 const auto& data) {
    ss << "  " << name << " = [";
    for(int i = 0; i < attrib.NumElements; ++i)
      ss << " " << core::format("{}", data[i]);
    ss << " ]\n";
  };

  if(hasPosition())
    attributeToString("Position", layout_.Position, getPosition());

  if(hasNormal())
    attributeToString("Normal", layout_.Normal, getNormal());

  if(hasTexCoord())
    attributeToString("TexCoord", layout_.TexCoord, getTexCoord());

  if(hasColor())
    attributeToString("Color", layout_.Color, getColor());

  ss << "]";
  return ss.str();
}

std::ostream& operator<<(std::ostream& os, const VertexAdapter& adapter) {
  return (os << adapter.toString());
}

} // namespace render

} // namespace sequoia
