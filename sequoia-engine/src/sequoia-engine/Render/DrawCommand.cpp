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

#include "sequoia-engine/Render/DrawCommand.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"

namespace sequoia {

namespace render {

std::string DrawCommand::toString() const {
  std::stringstream ss;
  ss << modelMatrix_;
  return core::format("DrawCommand[\n"
                      "  renderState = {},\n"
                      "  modelMatrix = Mat4[{}\n  ],\n"
                      "  uniformVariables = {}\n"
                      "]",
                      core::indent(state_.toString()), core::indent(ss.str(), 4),
                      variables_.empty()
                          ? "null"
                          : core::indent(core::toStringRange(variables_, [](const auto& var) {
                              return core::format("name = {},\n"
                                                  "variable = {}\n",
                                                  var.first, var.second.toString());
                            })));
}

} // namespace render

} // namespace sequoia
