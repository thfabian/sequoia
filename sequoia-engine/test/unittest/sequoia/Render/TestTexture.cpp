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

#include "sequoia/Render/Texture.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

TEST(TextureParameterTest, Comparison) {
  TextureParameter param1;
  TextureParameter param2;

  // If either of this test ever fails, this means some variables are not properly initialized
  EXPECT_EQ(param1, param1);
  EXPECT_EQ(param1, param2);

  param1.UseMipmap = !param1.UseMipmap;
  EXPECT_NE(param1, param2);
}

TEST(TextureParameterTest, Hash) {
  TextureParameter param1;
  TextureParameter param2;
  
  EXPECT_EQ(std::hash<TextureParameter>()(param1), std::hash<TextureParameter>()(param1));
  EXPECT_EQ(std::hash<TextureParameter>()(param1), std::hash<TextureParameter>()(param2));

  param1.UseMipmap = !param1.UseMipmap;
  EXPECT_NE(param1, param2);
  EXPECT_NE(std::hash<TextureParameter>()(param1), std::hash<TextureParameter>()(param2));
}

} // anonymous namespace
