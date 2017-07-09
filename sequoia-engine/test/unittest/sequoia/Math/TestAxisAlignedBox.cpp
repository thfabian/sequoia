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

#include "sequoia/Math/AxisAlignedBox.h"
#include <gtest/gtest.h>

using namespace sequoia;

namespace {

TEST(AxisAlignedBoxTest, Construction) {
  {
    math::AxisAlignedBox aab;
    EXPECT_TRUE(aab.isNull());
    EXPECT_FALSE(aab.isFinite());
    EXPECT_FALSE(aab.isInfinite());
  }

  {
    math::AxisAlignedBox aab;
    aab.setExtents(math::vec3(-1, -1, -1), math::vec3(1, 1, 1));
    EXPECT_FALSE(aab.isNull());
    EXPECT_TRUE(aab.isFinite());
    EXPECT_FALSE(aab.isInfinite());

    EXPECT_EQ(aab.getMinimum(), math::vec3(-1, -1, -1));
    EXPECT_EQ(aab.getMaximum(), math::vec3(1, 1, 1));
  }
}

TEST(AxisAlignedBoxTest, MergePoint) {
  math::AxisAlignedBox aab;
  aab.merge(math::vec3(1, 1, 1));
  EXPECT_FALSE(aab.isNull());

  aab.merge(math::vec3(3, 3, 3));
  EXPECT_EQ(aab.getMinimum(), math::vec3(1, 1, 1));
  EXPECT_EQ(aab.getMaximum(), math::vec3(3, 3, 3));

  aab.merge(math::vec3(2, 2, -2));
  EXPECT_EQ(aab.getMinimum(), math::vec3(1, 1, -2));
  EXPECT_EQ(aab.getMaximum(), math::vec3(3, 3, 3));
}

TEST(AxisAlignedBoxTest, MergeBox) {
  // TODO...
}

TEST(AxisAlignedBoxTest, Intersection) {
  // TODO...
}

} // anonymous namespace
