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

#include "sequoia-engine/Core/Casting.h"
#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Image.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gli/gli.hpp>
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

using namespace sequoia;
using namespace sequoia::unittest;

namespace {

TEST(ImageTest, PNGImage) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto file = env.getFile("sequoia-engine/Core/TestImage/Test.png");

  EXPECT_EQ(file->getNumBytes(), 193);

  std::shared_ptr<Image> loadedImage = Image::load(file);
  ASSERT_TRUE(isa<RegularImage>(loadedImage.get()));

  const RegularImage* image = dyn_cast<RegularImage>(loadedImage.get());

  EXPECT_EQ(image->getHeight(), 32);
  EXPECT_EQ(image->getWidth(), 32);
  EXPECT_EQ(image->getNumChannels(), 3);
  EXPECT_EQ(image->getColorFormat(), core::ColorFormat::BGR);

  // Top left is blue
  EXPECT_EQ(image->at(0, 0), core::makeColorRGB(0, 0, 255));

  // Bottom left is red
  EXPECT_EQ(image->at(image->getHeight() - 1, 0), core::makeColorRGB(255, 0, 0));

  // Bottom right is blue
  EXPECT_EQ(image->at(image->getHeight() - 1, image->getWidth() - 1),
            core::makeColorRGB(0, 0, 255));

  // Top right is green
  EXPECT_EQ(image->at(0, image->getWidth() - 1), core::makeColorRGB(0, 255, 0));
}

TEST(ImageTest, JPEGImage) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto file = env.getFile("sequoia-engine/Core/TestImage/Test.jpg");

  EXPECT_EQ(file->getNumBytes(), 905);

  std::shared_ptr<Image> loadedImage = Image::load(file);
  ASSERT_TRUE(isa<RegularImage>(loadedImage.get()));

  const RegularImage* image = dyn_cast<RegularImage>(loadedImage.get());

  EXPECT_EQ(image->getHeight(), 32);
  EXPECT_EQ(image->getWidth(), 32);
  EXPECT_EQ(image->getNumChannels(), 3);
  EXPECT_EQ(image->getColorFormat(), core::ColorFormat::BGR);

  // JPEG is a lossy format so we don't check the colors.. they are not even the same for
  // Win32/Linux..
}

TEST(ImageTest, BMPImage) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto file = env.getFile("sequoia-engine/Core/TestImage/Test.bmp");

  EXPECT_EQ(file->getNumBytes(), 714);

  std::shared_ptr<Image> loadedImage = Image::load(file);
  ASSERT_TRUE(isa<RegularImage>(loadedImage.get()));

  const RegularImage* image = dyn_cast<RegularImage>(loadedImage.get());

  EXPECT_EQ(image->getHeight(), 32);
  EXPECT_EQ(image->getWidth(), 32);
  EXPECT_EQ(image->getNumChannels(), 3);
  EXPECT_EQ(image->getColorFormat(), core::ColorFormat::BGR);

  // Top left is blue
  EXPECT_EQ(image->at(0, 0), core::makeColorRGB(0, 0, 255));

  // Bottom left is red
  EXPECT_EQ(image->at(image->getHeight() - 1, 0), core::makeColorRGB(255, 0, 0));

  // Bottom right is blue
  EXPECT_EQ(image->at(image->getHeight() - 1, image->getWidth() - 1),
            core::makeColorRGB(0, 0, 255));

  // Top right is green
  EXPECT_EQ(image->at(0, image->getWidth() - 1), core::makeColorRGB(0, 255, 0));
}

TEST(ImageTest, TiffImage) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto file = env.getFile("sequoia-engine/Core/TestImage/Test.tiff");

  EXPECT_EQ(file->getNumBytes(), 878);

  std::shared_ptr<Image> loadedImage = Image::load(file);
  ASSERT_TRUE(isa<RegularImage>(loadedImage.get()));

  const RegularImage* image = dyn_cast<RegularImage>(loadedImage.get());

  EXPECT_EQ(image->getHeight(), 32);
  EXPECT_EQ(image->getWidth(), 32);
  EXPECT_EQ(image->getNumChannels(), 3);
  EXPECT_EQ(image->getColorFormat(), core::ColorFormat::BGR);

  // Top left is blue
  EXPECT_EQ(image->at(0, 0), core::makeColorRGB(0, 0, 255));

  // Bottom left is red
  EXPECT_EQ(image->at(image->getHeight() - 1, 0), core::makeColorRGB(255, 0, 0));

  // Bottom right is blue
  EXPECT_EQ(image->at(image->getHeight() - 1, image->getWidth() - 1),
            core::makeColorRGB(0, 0, 255));

  // Top right is green
  EXPECT_EQ(image->at(0, image->getWidth() - 1), core::makeColorRGB(0, 255, 0));
}

TEST(ImageTest, DDSImage) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto file = env.getFile("sequoia-engine/Core/TestImage/Test.dds");

  std::shared_ptr<Image> loadedImage = Image::load(file);
  ASSERT_TRUE(isa<TextureImage>(loadedImage.get()));

  const TextureImage* image = dyn_cast<TextureImage>(loadedImage.get());

  const gli::texture& texture = image->getTexture();
  EXPECT_TRUE(gli::is_compressed(texture.format()));

  // Check mip-map levels
  ASSERT_EQ(texture.levels(), 6);

  EXPECT_EQ(texture.extent(0).x, 32);
  EXPECT_EQ(texture.extent(0).y, 32);

  EXPECT_EQ(texture.extent(1).x, 16);
  EXPECT_EQ(texture.extent(1).y, 16);

  EXPECT_EQ(texture.extent(2).x, 8);
  EXPECT_EQ(texture.extent(2).y, 8);

  EXPECT_EQ(texture.extent(3).x, 4);
  EXPECT_EQ(texture.extent(3).y, 4);

  EXPECT_EQ(texture.extent(4).x, 2);
  EXPECT_EQ(texture.extent(4).y, 2);

  EXPECT_EQ(texture.extent(5).x, 1);
  EXPECT_EQ(texture.extent(5).y, 1);
}

TEST(ImageTest, InvalidImage) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto file = env.getFile("sequoia-engine/Core/TestImage/Test.txt");
  EXPECT_THROW(Image::load(file), core::Exception);
}

TEST(ImageTest, Comparison) {
  TestEnvironment& env = TestEnvironment::getSingleton();

  std::shared_ptr<Image> image1 =
      Image::load(env.getFile("sequoia-engine/Core/TestImage/Test.png"));
  std::shared_ptr<Image> image2 = image1;
  EXPECT_EQ(*image1, *image2);

  std::shared_ptr<Image> image3 =
      Image::load(env.getFile("sequoia-engine/Core/TestImage/Test.bmp"));
  EXPECT_NE(*image1, *image3);

  std::shared_ptr<Image> image4 =
      Image::load(env.getFile("sequoia-engine/Core/TestImage/Test.dds"));
  std::shared_ptr<Image> image5 = image4;
  EXPECT_EQ(*image4, *image5);
}

} // anonymous namespace
