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

#ifndef SEQUOIA_ENGINE_UNITTEST_GAMESETUP_H
#define SEQUOIA_ENGINE_UNITTEST_GAMESETUP_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Unittest/Fixture.h"

namespace sequoia {

namespace unittest {

/// @brief Handle creation and initialization of the game object
/// @ingroup unittest
class SEQUOIA_API GameSetup : public NonCopyable {
  std::unique_ptr<game::Game> game_;

public:
  /// @brief Register and initialize the Game
  ///
  /// This uses the `TestOptions` to initialize the game.
  void SetUp();

  /// @brief Destroy the Game
  void TearDown();
};

} // namespace unittest

} // namespace sequoia

#endif
