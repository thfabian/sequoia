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

#include "sequoia/Core/Mutex.h"
#include <gtest/gtest.h>

using namespace sequoia;

namespace {

TEST(MutexTest, Lock) {
  SpinMutex mutex;
  SEQUOIA_LOCK(mutex);
  SEQUOIA_UNLOCK(mutex);
}

TEST(MutexTest, TryLock) {
  SpinMutex mutex;
  bool locked = SEQUOIA_TRY_LOCK(mutex);
  EXPECT_TRUE(locked);
  SEQUOIA_UNLOCK(mutex);
}

TEST(MutexTest, LockGuard) {
  SpinMutex mutex;
  SEQUOIA_LOCK_GUARD(mutex);
}

} // anonymous namespace
