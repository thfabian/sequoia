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

namespace {

TEST(ColorTest, ColorG) {
  static_assert(ColorG::NumChannels == 1, "");
  static_assert(ColorG::Format == core::ColorFormat::G, "");

  ColorG color1(12);
  EXPECT_EQ(color1[0], 12);
  EXPECT_EQ(color1.r, 12);
  EXPECT_EQ(color1.data[0], 12);
  EXPECT_EQ(color1.x, 12);
  EXPECT_TRUE(color1 == ColorG(12));

  Byte pixelData[4] = {255, 254, 253, 252};
  ColorG color2(pixelData);
  EXPECT_EQ(color2[0], 255);
  EXPECT_EQ(color2.r, 255);
  EXPECT_EQ(color2.x, 255);
  EXPECT_EQ(color2.data[0], 255);
  EXPECT_TRUE(color2 == ColorG(255));

  EXPECT_TRUE(color2 != color1);
  EXPECT_TRUE(color2 == color2);
}

TEST(ColorTest, ColorGA) {
  static_assert(ColorGA::NumChannels == 2, "");
  static_assert(ColorGA::Format == core::ColorFormat::GA, "");

  ColorGA color1(12, 13);
  EXPECT_EQ(color1[0], 12);
  EXPECT_EQ(color1.r, 12);
  EXPECT_EQ(color1[1], 13);
  EXPECT_EQ(color1.g, 13);
  EXPECT_TRUE(color1 == ColorGA(12, 13));

  Byte pixelData[4] = {255, 254, 253, 252};
  ColorGA color2(pixelData);
  EXPECT_EQ(color2[0], 255);
  EXPECT_EQ(color2[1], 254);
  EXPECT_EQ(color2.r, 255);
  EXPECT_EQ(color2.g, 254);

  EXPECT_TRUE(color2 != color1);
  EXPECT_TRUE(color2 == color2);
}

TEST(ColorTest, ColorRGB) {
  static_assert(ColorRGB::NumChannels == 3, "");
  static_assert(ColorRGB::Format == core::ColorFormat::RGB, "");

  ColorRGB color1(12, 13, 14);
  EXPECT_EQ(color1[0], 12);
  EXPECT_EQ(color1.r, 12);
  EXPECT_EQ(color1[1], 13);
  EXPECT_EQ(color1.g, 13);
  EXPECT_EQ(color1[2], 14);
  EXPECT_EQ(color1.b, 14);
  EXPECT_TRUE(color1 == ColorRGB(12, 13, 14));

  Byte pixelData[4] = {255, 254, 253, 252};
  ColorRGB color2(pixelData);
  EXPECT_EQ(color2[0], 255);
  EXPECT_EQ(color2[1], 254);
  EXPECT_EQ(color2[2], 253);
  EXPECT_EQ(color2.r, 255);
  EXPECT_EQ(color2.g, 254);
  EXPECT_EQ(color2.b, 253);

  EXPECT_TRUE(color2 != color1);
  EXPECT_TRUE(color2 == color2);
}

TEST(ColorTest, ColorRGBA) {
  static_assert(ColorRGBA::NumChannels == 4, "");
  static_assert(ColorRGBA::Format == core::ColorFormat::RGBA, "");

  ColorRGBA color1(12, 13, 14, 15);
  EXPECT_EQ(color1[0], 12);
  EXPECT_EQ(color1.r, 12);
  EXPECT_EQ(color1[1], 13);
  EXPECT_EQ(color1.g, 13);
  EXPECT_EQ(color1[2], 14);
  EXPECT_EQ(color1.b, 14);
  EXPECT_EQ(color1[3], 15);
  EXPECT_EQ(color1.a, 15);
  EXPECT_TRUE(color1 == ColorRGBA(12, 13, 14, 15));

  Byte pixelData[4] = {255, 254, 253, 252};
  ColorRGBA color2(pixelData);
  EXPECT_EQ(color2[0], 255);
  EXPECT_EQ(color2[1], 254);
  EXPECT_EQ(color2[2], 253);
  EXPECT_EQ(color2[3], 252);
  EXPECT_EQ(color2.r, 255);
  EXPECT_EQ(color2.g, 254);
  EXPECT_EQ(color2.b, 253);
  EXPECT_EQ(color2.a, 252);

  EXPECT_TRUE(color2 != color1);
  EXPECT_TRUE(color2 == color2);
}

} // anonymous namespace
