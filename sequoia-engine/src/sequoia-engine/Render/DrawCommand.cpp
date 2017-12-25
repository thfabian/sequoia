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
#include "sequoia-engine/Render/DrawCommand.h"
#include <sstream>

namespace sequoia {

namespace render {

std::string DrawCommand::toString() const {
  std::stringstream ss;
  ss << modelMatrix_;
  return core::format(
      "DrawCommand[\n"
      "  data = {},\n"
      "  modelMatrix = Mat4[{}\n  ],\n"
      "  textures = {},\n"
      "  uniforms = {}\n"
      "]",
      data_ ? core::indent(data_->toString()) : "null", core::indent(ss.str(), 4),
      textures_.empty() ? "null" : core::indent(core::toStringRange(
                                       textures_,
                                       [](const auto& var) {
                                         return core::indent(core::format(
                                             "texture = {{\n"
                                             "  unit = {},\n"
                                             "  texture = {}\n"
                                             "}}",
                                             var.first, core::indent(var.second->toString())));
                                       })),
      uniforms_.empty() ? "null" : core::indent(core::toStringRange(uniforms_, [](const auto& var) {
        return core::indent(core::format("uniform = {{\n"
                                         "  name = {},\n"
                                         "  variable = {}\n"
                                         "}}",
                                         var.first, core::indent(var.second.toString())));
      })));
}

} // namespace render

} // namespace sequoia
