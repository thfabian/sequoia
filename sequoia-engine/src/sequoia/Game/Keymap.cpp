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
#include "sequoia/Game/Keymap.h"

namespace sequoia {

namespace game {

std::string Keymap::toString() const {
  return core::format("Keymap[\n"
                      "  key = %s,\n"
                      "  button = %s,\n"
                      "  mod = %s\n"
                      "]",
                      key_, button_, mod_);
}

} // namespace game

} // namespace sequoia
