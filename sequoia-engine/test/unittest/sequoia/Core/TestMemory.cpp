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

#include "sequoia/Core/Memory.h"
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace {

TEST(MemoryTest, AlignedAlloc) {
  auto p = memory::aligned_alloc(1024);
  EXPECT_TRUE(memory::is_aligned(p));
  EXPECT_NE(p, nullptr);
  memory::aligned_free(p);
}

} // anonymous namespace
