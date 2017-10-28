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

#include "sequoia-engine/Unittest/Test.h"
#include "sequoia-engine/Core/Color.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;

namespace {

TEST(ColorTest, ColorRGB) {
  Color color1 = core::makeColorRGB(1, 2, 3);
  EXPECT_EQ(color1.getFormat(), core::ColorFormat::RGB);

  EXPECT_EQ(color1.r(), 1);
  EXPECT_EQ(color1.g(), 2);
  EXPECT_EQ(color1.b(), 3);
  EXPECT_EQ(color1.a(), 0);
  EXPECT_TRUE(color1 == core::makeColorRGB(1, 2, 3));

  Byte pixelData[4] = {255, 254, 253, 252};
  Color color2(core::ColorFormat::RGB, pixelData);
  EXPECT_EQ(color2[0], 255);
  EXPECT_EQ(color2[1], 254);
  EXPECT_EQ(color2[2], 253);
  EXPECT_EQ(color2[3], 0);

  EXPECT_TRUE(color2 != color1);
  EXPECT_TRUE(color2 == color2);
}

TEST(ColorTest, ColorRGBA) {
  Color color = core::makeColorRGBA(1, 2, 3, 4);
  EXPECT_EQ(color.getFormat(), core::ColorFormat::RGBA);

  EXPECT_EQ(color.r(), 1);
  EXPECT_EQ(color.g(), 2);
  EXPECT_EQ(color.b(), 3);
  EXPECT_EQ(color.a(), 4);
  EXPECT_TRUE(color == core::makeColorRGBA(1, 2, 3, 4));
}

TEST(ColorTest, ColorBGR) {
  Color color = core::makeColorBGR(1, 2, 3);
  EXPECT_EQ(color.getFormat(), core::ColorFormat::BGR);

  EXPECT_EQ(color.b(), 1);
  EXPECT_EQ(color.g(), 2);
  EXPECT_EQ(color.r(), 3);
  EXPECT_EQ(color.a(), 0);
  EXPECT_TRUE(color == core::makeColorBGR(1, 2, 3));
}

TEST(ColorTest, ColorBGRA) {
  Color color = core::makeColorBGRA(1, 2, 3, 4);
  EXPECT_EQ(color.getFormat(), core::ColorFormat::BGRA);

  EXPECT_EQ(color.b(), 1);
  EXPECT_EQ(color.g(), 2);
  EXPECT_EQ(color.r(), 3);
  EXPECT_EQ(color.a(), 4);
  EXPECT_TRUE(color == core::makeColorBGRA(1, 2, 3, 4));
}

TEST(ColorTest, Comparison) {
  EXPECT_EQ(core::makeColorRGBA(1, 2, 3, 4), core::makeColorBGRA(3, 2, 1, 4));
  EXPECT_EQ(core::makeColorRGB(1, 2, 3), core::makeColorBGR(3, 2, 1));
  EXPECT_NE(core::makeColorRGB(1, 2, 3), core::makeColorRGBA(1, 2, 3, 4));
}

TEST(ColorTest, Normalize) {
  EXPECT_VEC_NEAR(math::make_vec4(core::makeColorRGBA(51, 102, 153, 204).normalize().data()),
                  math::vec4(0.2, 0.4, 0.6, 0.8), 1e-05f);

  EXPECT_VEC_NEAR(math::make_vec4(core::makeColorBGRA(51, 102, 153, 204).normalize().data()),
                  math::vec4(0.6, 0.4, 0.2, 0.8), 1e-05f);
}

} // anonymous namespace
