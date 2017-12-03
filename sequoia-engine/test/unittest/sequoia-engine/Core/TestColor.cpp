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

#include "sequoia-engine/Core/Color.h"
#include "sequoia-engine/Unittest/Test.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;

namespace {

TEST(ColorTest, Construction) {
  Color color1(1, 2, 3);
  EXPECT_EQ(color1.r, 1);
  EXPECT_EQ(color1.g, 2);
  EXPECT_EQ(color1.b, 3);
  EXPECT_EQ(color1.a, 255);
  
  Byte pixelData[4] = {255, 254, 253, 252};
  Color color2(pixelData);
  EXPECT_EQ(color2[0], 255);
  EXPECT_EQ(color2[1], 254);
  EXPECT_EQ(color2[2], 253);
  EXPECT_EQ(color2[3], 252);

  EXPECT_TRUE(color2 != color1);
  EXPECT_TRUE(color2 == color2);
}

TEST(ColorTest, RGBA32f) {
  EXPECT_VEC_NEAR(math::make_vec4(Color(51, 102, 153, 204).toRGBA32f().data()),
                  math::vec4(0.2, 0.4, 0.6, 0.8), 1e-05f);
}

} // anonymous namespace
