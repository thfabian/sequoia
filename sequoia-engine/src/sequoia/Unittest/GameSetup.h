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

#ifndef SEQUOIA_UNITTEST_GAMESETUP_H
#define SEQUOIA_UNITTEST_GAMESETUP_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Unittest/Fixture.h"

namespace sequoia {

namespace unittest {

/// @brief Handle creation and initialization of the game object
/// @ingroup unittest
class SEQUOIA_API GameSetup : public NonCopyable {
  std::unique_ptr<game::Game> game_;

public:
  /// @brief Register and initialize the Game
  void SetUp();

  /// @brief Destroy the Game
  void TearDown();
};

#define SEQUOIA_GAME_TEST_FIXTURE(Name) SEQUOIA_TEST_FIXTURE(Name, sequoia::unittest::GameSetup)
#define SEQUOIA_GAME_BENCHMARK_FIXTURE(Name)                                                       \
  SEQUOIA_BENCHMARK_FIXTURE(Name, sequoia::unittest::GameSetup)

} // namespace unittest

} // namespace sequoia

#endif
