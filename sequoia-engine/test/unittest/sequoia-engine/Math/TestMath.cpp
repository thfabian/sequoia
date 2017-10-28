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

#include "sequoia-engine/Math/Math.h"
#include <gtest/gtest.h>

using namespace sequoia;

namespace {

TEST(MathTest, Degree) {
  math::Degree deg(45.0f);

  EXPECT_FLOAT_EQ(deg.inDegrees(), 45.0f);
  EXPECT_FLOAT_EQ(deg.inRadians(), math::Radian::fromDegree(45.0f).inRadians());

  deg += math::Degree(45.0f);
  EXPECT_FLOAT_EQ(deg.inDegrees(), 90.0f);
  EXPECT_FLOAT_EQ(deg.inRadians(), math::Radian::fromDegree(90.0f).inRadians());
}

TEST(MathTest, Radian) {
  math::Radian rad(math::constantsf::pi);

  EXPECT_FLOAT_EQ(rad.inRadians(), math::constantsf::pi);
  EXPECT_FLOAT_EQ(rad.inDegrees(), math::Degree::fromRadian(math::constantsf::pi).inDegrees());

  rad += math::Radian(math::constantsf::pi / 2.0f);
  EXPECT_FLOAT_EQ(rad.inRadians(), 3.0f / 2.0f * math::constantsf::pi);
  EXPECT_FLOAT_EQ(rad.inDegrees(),
                  math::Degree::fromRadian(3.0f / 2.0f * math::constantsf::pi).inDegrees());
}

} // anonymous namespace
