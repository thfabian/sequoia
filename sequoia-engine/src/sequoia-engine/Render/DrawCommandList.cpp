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

#include "sequoia-engine/Render/DrawCommandList.h"
#include "sequoia-engine/Core/StringUtil.h"
#include <sstream>

namespace sequoia {

namespace render {

std::string DrawCommandListDefault::toString() const noexcept {
  std::stringstream ss;
  ss << "DrawCommandListDefault[\n";
  for(const auto& cmd : commands_)
    ss << core::indent(cmd->toString()) << "\n";
  ss << "]";
  return ss.str();
}

} // namespace render

} // namespace sequoia