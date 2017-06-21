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
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/DrawCommand.h"

namespace sequoia {

namespace render {

std::string DrawCommand::toString() const {
  std::stringstream ss;
  ss << modelMatrix_;
  return core::format("DrawCommand[\n"
                      "  renderState = %s,\n"
                      "  modelMatrix = Mat4[%s\n  ],\n"
                      "  uniformVariables = %s\n"
                      "]",
                      core::indent(state_.toString()), core::indent(ss.str(), 4),
                      variables_.empty()
                          ? "null"
                          : core::indent(core::toStringRange(variables_, [](const auto& var) {
                              return core::format("name = %s,\n"
                                                  "variable = %s\n",
                                                  var.first, var.second.toString());
                            })));
}

} // namespace render

} // namespace sequoia
