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
  // Allocate Singleton
  constructionCounter = 0;
  SingletonManager::getSingleton().allocateSingleton<DummySingleton>();
  int numSingletons = SingletonManager::getSingleton().numSingletons();
  EXPECT_EQ(constructionCounter, 1);

  // Deallocate Singleton
  destructionCounter = 0;
  SingletonManager::getSingleton().freeSingleton<DummySingleton>();
  EXPECT_EQ(destructionCounter, 1);
  EXPECT_EQ(SingletonManager::getSingleton().numSingletons(), numSingletons - 1);
}

} // anonymous namespace

SEQUOIA_DECLARE_SINGLETON(DummySingleton);
