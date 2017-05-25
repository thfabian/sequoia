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

#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/DrawCommandList.h"
#include <sstream>

namespace sequoia {

namespace render {

std::string DrawCommandListDefault::toString() const noexcept {
  std::stringstream ss;
  ss << "DrawCommandListDefault[\n";
  for(DrawCommand* cmd : commands_)
    ss << core::indent(cmd->toString()) << "\n";
  ss << "]";
  return ss.str();
}

} // namespace render

} // namespace sequoia
