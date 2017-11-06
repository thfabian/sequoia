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

#include "sequoia-engine/Unittest/GameSetup.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"

namespace sequoia {

namespace unittest {

void GameSetup::SetUp() {
  auto& env = TestEnvironment::getSingleton();

  game_ = std::make_unique<game::Game>();
  game::GameOptions options(&env.getOptions(), env.getRenderSystemKind());
  options.HideWindow = true;
  options.Name = "GameUnittest";
  game_->init(options);
}

void GameSetup::TearDown() { game_.reset(); }

} // namespace unittest

} // namespace sequoia
