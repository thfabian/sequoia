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
#include "sequoia/Unittest/GameTest.h"
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

  // Load ASCII file
  auto file = manager.load("sequoia/Game/TestAssetManager/test.txt");

  std::ifstream ifs(
      platform::toAnsiString(env.getRessourcePath() /
                             platform::asPath("sequoia/Game/TestAssetManager/test.txt"))
          .c_str());
  std::stringstream ss;
  ss << ifs.rdbuf();

  EXPECT_STREQ(file->getDataAsString().c_str(), ss.str().c_str());
}

} // anonymous namespace
