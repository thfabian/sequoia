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

#include "sequoia/Render/DrawCommand.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"

namespace sequoia {

namespace render {

std::string DrawCommand::toString() const {
  std::stringstream ss;
  ss << modelMatrix_;
  return core::format("DrawCommand[\n"
                      "  renderState = %s,\n"
                      "  modelMatrix = %s\n"
                      "]",
                      core::indent(state_.toString()), core::indent(ss.str()));
}

} // namespace render

} // namespace sequoia
