//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/SingletonManager.h"
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace {

static int destructionCounter = 0;
static int constructionCounter = 0;

class DummySingleton : public Singleton<DummySingleton> {
public:
  DummySingleton() { constructionCounter += 1; }
  ~DummySingleton() { destructionCounter += 1; }
};

TEST(SingletonManagerTest, ManualFree) {
  auto singletonManager = std::make_unique<SingletonManager>();
  (void)singletonManager;
  EXPECT_EQ(SingletonManager::getSingleton().numSingletons(), 0);

  // Allocate Singleton
  constructionCounter = 0;
  SingletonManager::getSingleton().allocateSingleton<DummySingleton>();
  EXPECT_EQ(constructionCounter, 1);
  EXPECT_EQ(SingletonManager::getSingleton().numSingletons(), 1);

  // Deallocate Singleton
  destructionCounter = 0;
  SingletonManager::getSingleton().freeSingleton<DummySingleton>();
  EXPECT_EQ(destructionCounter, 1);
  EXPECT_EQ(SingletonManager::getSingleton().numSingletons(), 0);
}

TEST(SingletonManagerTest, AutoFree) {
  destructionCounter = constructionCounter = 0;

  {
    auto singletonManager = std::make_unique<SingletonManager>();
    SingletonManager::getSingleton().allocateSingleton<DummySingleton>();
  }

  EXPECT_EQ(constructionCounter, 1);
  EXPECT_EQ(destructionCounter, 1);
}

} // anonymous namespace

SEQUIOA_DECLARE_SINGLETON(DummySingleton);
