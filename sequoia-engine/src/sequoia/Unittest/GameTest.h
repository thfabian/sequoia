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

#ifndef SEQUOIA_UNITTEST_GAMETEST_H
#define SEQUOIA_UNITTEST_GAMETEST_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Unittest/Export.h"
#include <gtest/gtest.h>

namespace sequoia {

namespace unittest {

/// @brief Handle creation and initialization of the game object
/// @ingroup unittest
class SEQUOIA_API GameTest : public testing::Test, public NonCopyable {
  std::unique_ptr<game::Game> game_;

protected:
  /// @brief Register and initialize the Game
  void SetUp();

  /// @brief Destroy the Game
  void TearDown();
};

} // namespace unittest

} // namespace sequoia

#endif
