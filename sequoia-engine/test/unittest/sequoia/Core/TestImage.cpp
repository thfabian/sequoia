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
#include "sequoia/Core/Casting.h"
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

  // Bottom left is red
  EXPECT_EQ(image->at(0, 0), core::makeColorRGB(255, 0, 0));

  // Bottom right is blue
  EXPECT_EQ(image->at(0, image->getWidth() - 1), core::makeColorRGB(0, 0, 255));

  // Top left is blue
  EXPECT_EQ(image->at(image->getHeight() - 1, 0), core::makeColorRGB(0, 0, 255));

  // Top right is green
  EXPECT_EQ(image->at(image->getHeight() - 1, image->getWidth() - 1),
            core::makeColorRGB(0, 255, 0));
}

TEST(ImageTest, JPEGImage) {
  Environment& env = Environment::getSingleton();
  auto file = env.getFile("sequoia/Core/TestImage/Test.jpg");

  EXPECT_EQ(file->getNumBytes(), 905);

  std::shared_ptr<Image> image = Image::load(file);
  EXPECT_EQ(image->getHeight(), 32);
  EXPECT_EQ(image->getWidth(), 32);
  EXPECT_EQ(image->getNumChannels(), 3);
  EXPECT_EQ(image->getColorFormat(), core::ColorFormat::BGR);

  // JPEG is a lossy format so we don't get the exact colors

  // Bottom left is red
  EXPECT_EQ(image->at(0, 0), core::makeColorRGB(255, 2, 0));

  // Bottom right is blue
  EXPECT_EQ(image->at(0, image->getWidth() - 1), core::makeColorRGB(0, 6, 255));

  // Top left is blue
  EXPECT_EQ(image->at(image->getHeight() - 1, 0), core::makeColorRGB(0, 4, 255));

  // Top right is green
  EXPECT_EQ(image->at(image->getHeight() - 1, image->getWidth() - 1),
            core::makeColorRGB(4, 254, 6));

  // (0, 5) is yellow
  EXPECT_EQ(image->at(0, 4), core::makeColorRGB(255, 250, 11));
}

TEST(ImageTest, BMPImage) {
  Environment& env = Environment::getSingleton();
  auto file = env.getFile("sequoia/Core/TestImage/Test.bmp");

  EXPECT_EQ(file->getNumBytes(), 714);

  std::shared_ptr<Image> image = Image::load(file);
  EXPECT_EQ(image->getHeight(), 32);
  EXPECT_EQ(image->getWidth(), 32);
  EXPECT_EQ(image->getNumChannels(), 3);
  EXPECT_EQ(image->getColorFormat(), core::ColorFormat::BGR);

  // Bottom left is red
  EXPECT_EQ(image->at(0, 0), core::makeColorRGB(255, 0, 0));

  // Bottom right is blue
  EXPECT_EQ(image->at(0, image->getWidth() - 1), core::makeColorRGB(0, 0, 255));

  // Top left is blue
  EXPECT_EQ(image->at(image->getHeight() - 1, 0), core::makeColorRGB(0, 0, 255));

  // Top right is green
  EXPECT_EQ(image->at(image->getHeight() - 1, image->getWidth() - 1),
            core::makeColorRGB(0, 255, 0));
}

TEST(ImageTest, DDSImage) {
  Environment& env = Environment::getSingleton();
  auto file = env.getFile("sequoia/Core/TestImage/Test.dds");
  std::shared_ptr<Image> image = Image::load(file);
  
  EXPECT_EQ(image->getHeight(), 32);
  EXPECT_EQ(image->getWidth(), 32);
  
//  const core::DDSImage* ddsImage = dyn_cast<core::DDSImage>(image.get());
//  EXPECT_EQ(image->get(), 6);

  //  std::cout << image->toString() << std::endl;
}

TEST(ImageTest, InvalidImage) {
  Environment& env = Environment::getSingleton();
  auto file = env.getFile("sequoia/Core/TestImage/Test.txt");
  EXPECT_THROW(Image::load(file), core::Exception);
}

TEST(ImageTest, Comparison) {
  Environment& env = Environment::getSingleton();

  std::shared_ptr<Image> image1 = Image::load(env.getFile("sequoia/Core/TestImage/Test.png"));
  std::shared_ptr<Image> image2 = image1;
  EXPECT_EQ(*image1, *image2);

  std::shared_ptr<Image> image3 = Image::load(env.getFile("sequoia/Core/TestImage/Test.bmp"));
  EXPECT_NE(*image1, *image3);
}

} // anonymous namespace
