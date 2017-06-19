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

#include "sequoia/Core/Color.h"
#include "sequoia/Unittest/Environment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::core;

namespace {

TEST(ColorTest, ColorG) {
  static_assert(ColorG::NumChannels == 1, "");
  static_assert(ColorG::Format == ColorFormat::G, "");

  ColorG color1(12);
  EXPECT_EQ(color1[0], 12);
  EXPECT_EQ(color1.r(), 12);
  EXPECT_EQ(color1.getChannel<0>(), 12);
  EXPECT_EQ(color1.getChannel(0), 12);
  EXPECT_TRUE(color1 == ColorG(12));

  Byte pixelData[4] = {255, 254, 253, 252};
  ColorG color2(pixelData);
  EXPECT_EQ(color2[0], 255);
  EXPECT_EQ(color2.r(), 255);
  EXPECT_EQ(color2.getChannel<0>(), 255);
  EXPECT_EQ(color2.getChannel(0), 255);
  EXPECT_TRUE(color2 == ColorG(255));

  EXPECT_TRUE(color2 != color1);
  EXPECT_TRUE(color2 == color2);
}

} // anonymous namespace
