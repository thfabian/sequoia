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

#include "sequoia-engine/Game/AssetManager.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

TEST(AssetManagerTest, LoadFromDisk) {
  auto& env = TestEnvironment::getSingleton();
  AssetManager manager(platform::toAnsiString(env.getRessourcePath()));

  auto file = manager.load("sequoia-engine/Game/TestAssetManager/Test.txt");

  std::ifstream ifs(
      platform::toAnsiString(env.getRessourcePath() /
                             platform::asPath("sequoia-engine/Game/TestAssetManager/Test.txt"))
          .c_str());
  std::stringstream ss;
  ss << ifs.rdbuf();

  EXPECT_STREQ(file->getDataAsString().c_str(), ss.str().c_str());
  EXPECT_STREQ(file->getExtension().c_str(), ".txt");
  EXPECT_STREQ(file->getFilename().c_str(), "Test.txt");
}

TEST(AssetManagerTest, LoadImage) {
  auto& env = TestEnvironment::getSingleton();
  AssetManager manager(platform::toAnsiString(env.getRessourcePath()));
  
  auto image = manager.loadImage("sequoia-engine/Game/TestAssetManager/Test.png");
  auto imageCopy = manager.loadImage("sequoia-engine/Game/TestAssetManager/Test.png");
  EXPECT_EQ(*image, *imageCopy);
}

} // anonymous namespace
