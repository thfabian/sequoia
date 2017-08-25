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

#include "sequoia/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
  // Initialize gtest
  testing::InitGoogleTest(&argc, argv);

  // Register test environment
  testing::AddGlobalTestEnvironment(
      new sequoia::unittest::TestEnvironment(argc, argv, sequoia::render::RK_Null));

  return RUN_ALL_TESTS();
}
