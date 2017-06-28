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

#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Image.h"
#include "sequoia/Unittest/Environment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;

namespace {

TEST(ImageTest, PNGImage) {
  Environment& env = Environment::getSingleton();
  auto file = env.getFile("sequoia/Core/TestImage/Test.png");

  EXPECT_EQ(file->getNumBytes(), 193);

  std::shared_ptr<Image> image = Image::load(file);
  EXPECT_EQ(image->getHeight(), 32);
  EXPECT_EQ(image->getWidth(), 32);
  EXPECT_EQ(image->getNumChannels(), 3);
  EXPECT_EQ(image->getColorFormat(), core::ColorFormat::BGR);

//  // Top left is blue
//  EXPECT_EQ(image->at<ColorRGB>(0, 0), ColorRGB(0, 0, 255));
  
  //EXPECT_EQ(image->at<ColorRGB>(0, image->getWidth() - 1), ColorRGB(0, 255, 0));

//  // Bottom left is red
//  EXPECT_EQ(image->at<ColorRGB>(image->getHeight() - 1, 0), ColorRGB(255, 0, 0));

//  // Bottom right is red
//  EXPECT_EQ(image->at<ColorRGB>(image->getHeight() - 1, image->getWidth() - 1),
//            ColorRGB(0, 0, 255));

//  // (0, 8) is yellow
//  EXPECT_EQ(image->at<ColorRGB>(0, 8), ColorRGB(255, 255, 0));
}

//TEST(ImageTest, JPEGImage) {
//  Environment& env = Environment::getSingleton();
//  auto file = env.getFile("sequoia/Core/TestImage/Test.jpg");

//  EXPECT_EQ(file->getNumBytes(), 905);

//  std::shared_ptr<Image> image = Image::load(file);
//  EXPECT_EQ(image->getHeight(), 32);
//  EXPECT_EQ(image->getWidth(), 32);
//  EXPECT_EQ(image->getNumChannels(), 3);
//  EXPECT_EQ(image->getColorFormat(), core::ColorFormat::RGB);

//  // JPEG is a lossy format so we don't get the exact colors

//  // Top left is blue
//  EXPECT_EQ(image->at<ColorRGB>(0, 0), ColorRGB(0, 3, 254));

//  // Top right is green
//  EXPECT_EQ(image->at<ColorRGB>(0, image->getWidth() - 1), ColorRGB(2, 250, 4));

//  // Bottom left is red
//  EXPECT_EQ(image->at<ColorRGB>(image->getHeight() - 1, 0), ColorRGB(252, 0, 0));

//  // Bottom right is red
//  EXPECT_EQ(image->at<ColorRGB>(image->getHeight() - 1, image->getWidth() - 1),
//            ColorRGB(0, 3, 254));

//  // (0, 8) is yellow
//  EXPECT_EQ(image->at<ColorRGB>(0, 8), ColorRGB(255, 231, 41));
//}

//TEST(ImageTest, BMPImage) {
//  Environment& env = Environment::getSingleton();
//  auto file = env.getFile("sequoia/Core/TestImage/Test.bmp");

//  EXPECT_EQ(file->getNumBytes(), 714);

//  std::shared_ptr<Image> image = Image::load(file);
//  EXPECT_EQ(image->getHeight(), 32);
//  EXPECT_EQ(image->getWidth(), 32);
//  EXPECT_EQ(image->getNumChannels(), 4);
//  EXPECT_EQ(image->getColorFormat(), core::ColorFormat::RGBA);

//  // Top left is blue
//  EXPECT_EQ(image->at<ColorRGB>(0, 0), ColorRGB(0, 0, 255));

//  // Top right is green
//  EXPECT_EQ(image->at<ColorRGB>(0, image->getWidth() - 1), ColorRGB(0, 255, 0));

//  // Bottom left is red
//  EXPECT_EQ(image->at<ColorRGB>(image->getHeight() - 1, 0), ColorRGB(255, 0, 0));

//  // Bottom right is red
//  EXPECT_EQ(image->at<ColorRGB>(image->getHeight() - 1, image->getWidth() - 1),
//            ColorRGB(0, 0, 255));

//  // (0, 8) is yellow
//  EXPECT_EQ(image->at<ColorRGB>(0, 8), ColorRGB(255, 255, 0));
//}

//TEST(ImageTest, InvalidImage) {
//  Environment& env = Environment::getSingleton();
//  auto file = env.getFile("sequoia/Core/TestImage/Test.txt");
//  EXPECT_THROW(Image::load(file), core::Exception);
//}

//TEST(ImageTest, Comparison) {
//  Environment& env = Environment::getSingleton();

//  std::shared_ptr<Image> image1 = Image::load(env.getFile("sequoia/Core/TestImage/Test.bmp"));
//  std::shared_ptr<Image> image2 = image1;

//  EXPECT_EQ(*image1, *image2);

//  std::shared_ptr<Image> image3 = Image::load(env.getFile("sequoia/Core/TestImage/Test.png"));
//  EXPECT_NE(*image1, *image3);
//}

} // anonymous namespace
