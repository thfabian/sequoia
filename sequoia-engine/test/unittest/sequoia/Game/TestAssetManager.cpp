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

#include "sequoia/Game/AssetManager.h"
#include "sequoia/Unittest/Environment.h"
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

TEST(AssetManagerTest, LoadFromDisk) {
  auto& env = Environment::getSingleton();
  AssetManager manager(env.getRessourcePath().native(), PLATFORM_STR(""));

  auto file = manager.load("sequoia/Game/TestAssetManager/Test.txt");

  std::ifstream ifs(
      platform::toAnsiString(env.getRessourcePath() /
                             platform::asPath("sequoia/Game/TestAssetManager/Test.txt"))
          .c_str());
  std::stringstream ss;
  ss << ifs.rdbuf();

  EXPECT_STREQ(file->getDataAsString().c_str(), ss.str().c_str());
  EXPECT_STREQ(file->getExtension().str().c_str(), ".txt");
  EXPECT_STREQ(file->getFilename().str().c_str(), "Test.txt");
}

TEST(AssetManagerTest, LoadImage) {
  auto& env = Environment::getSingleton();
  AssetManager manager(env.getRessourcePath().native(), PLATFORM_STR(""));
  
  auto image = manager.loadImage("sequoia/Game/TestAssetManager/Test.png");
  auto imageCopy = manager.loadImage("sequoia/Game/TestAssetManager/Test.png");
  EXPECT_EQ(*image, *imageCopy);
}

} // anonymous namespace
