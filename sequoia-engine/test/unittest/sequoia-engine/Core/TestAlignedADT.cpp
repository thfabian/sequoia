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

#include "sequoia-engine/Core/AlignedADT.h"
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace {

TEST(AlignedADTTest, Vector) {
  aligned_vector<double> vec;
  EXPECT_TRUE(memory::is_aligned(vec.data()));
  vec.push_back(1.0);
  vec.push_back(2.0);
  EXPECT_EQ(vec.size(), 2);
}

} // anonymous namespace
