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
  EXPECT_EQ(image.at<ColorRGB>(0, 0), ColorRGB(0, 0, 255));

  // Top right is green
  EXPECT_EQ(image.at<ColorRGB>(0, image.getWidth() - 1), ColorRGB(0, 255, 0));

  // Bottom left is red
  EXPECT_EQ(image.at<ColorRGB>(image.getHeight() - 1, 0), ColorRGB(255, 0, 0));

  // Bottom right is red
  EXPECT_EQ(image.at<ColorRGB>(image.getHeight() - 1, image.getWidth() - 1), ColorRGB(0, 0, 255));

  // (0, 8) is yellow
  EXPECT_EQ(image.at<ColorRGB>(0, 8), ColorRGB(255, 255, 0));
}

TEST(ImageTest, LoadJPEG) {
  Environment& env = Environment::getSingleton();
  auto file = env.getFile("sequoia/Core/TestImage/test.jpg");

  EXPECT_EQ(file->getNumBytes(), 905);

  Image image(file);
  EXPECT_EQ(image.getHeight(), 32);
  EXPECT_EQ(image.getWidth(), 32);
  EXPECT_EQ(image.getNumChannels(), 3);

  // JPEG is a lossy format so we don't get the exact colors

  // Top left is blue
  EXPECT_EQ(image.at<ColorRGB>(0, 0), ColorRGB(0, 3, 254));

  // Top right is green
  EXPECT_EQ(image.at<ColorRGB>(0, image.getWidth() - 1), ColorRGB(2, 250, 4));

  // Bottom left is red
  EXPECT_EQ(image.at<ColorRGB>(image.getHeight() - 1, 0), ColorRGB(252, 0, 0));

  // Bottom right is red
  EXPECT_EQ(image.at<ColorRGB>(image.getHeight() - 1, image.getWidth() - 1), ColorRGB(0, 3, 254));

  // (0, 8) is yellow
  EXPECT_EQ(image.at<ColorRGB>(0, 8), ColorRGB(255, 231, 41));
}

TEST(ImageTest, LoadBMP) {
  Environment& env = Environment::getSingleton();
  auto file = env.getFile("sequoia/Core/TestImage/test.bmp");

  EXPECT_EQ(file->getNumBytes(), 714);

  Image image(file);
  EXPECT_EQ(image.getHeight(), 32);
  EXPECT_EQ(image.getWidth(), 32);
  EXPECT_EQ(image.getNumChannels(), 4);

  // Top left is blue
  EXPECT_EQ(image.at<ColorRGB>(0, 0), ColorRGB(0, 0, 255));

  // Top right is green
  EXPECT_EQ(image.at<ColorRGB>(0, image.getWidth() - 1), ColorRGB(0, 255, 0));

  // Bottom left is red
  EXPECT_EQ(image.at<ColorRGB>(image.getHeight() - 1, 0), ColorRGB(255, 0, 0));

  // Bottom right is red
  EXPECT_EQ(image.at<ColorRGB>(image.getHeight() - 1, image.getWidth() - 1), ColorRGB(0, 0, 255));

  // (0, 8) is yellow
  EXPECT_EQ(image.at<ColorRGB>(0, 8), ColorRGB(255, 255, 0));
}

} // anonymous namespace
