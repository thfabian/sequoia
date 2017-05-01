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

#include "sequoia/Core/Options.h"
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace {

TEST(OptionsTest, Reset) {
  auto opt = std::make_unique<Options>();

  bool defaultVal = Options::getSingleton().Core.Logging;

  // Change the option
  Options::getSingleton().Core.Logging = !defaultVal;

  // Reset
  Options::getSingleton().reset();
  EXPECT_EQ(defaultVal, Options::getSingleton().Core.Logging);
}

} // anonymous namespace
