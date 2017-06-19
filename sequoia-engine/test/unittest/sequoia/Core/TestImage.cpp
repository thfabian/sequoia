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

#include "sequoia/Core/Image.h"
#include "sequoia/Unittest/Environment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::core;

namespace {

TEST(ImageTest, LoadPNG) {
  Environment& env = Environment::getSingleton();
  auto file = env.getFile("sequoia/Core/TestImage/test.png");
  
  EXPECT_EQ(file->getNumBytes(), 193);

  Image image(file);
  EXPECT_EQ(image.getHeight(), 32);
  EXPECT_EQ(image.getWidth(), 32);
  EXPECT_EQ(image.getNumChannels(), 3);
  
  // Top left is blue
  
  // Top right is green
  
  // Bottom left is red
}

} // anonymous namespace
