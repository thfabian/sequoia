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

#include "sequoia/Render/UniformVariable.h"
#include "sequoia/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;

namespace {

TEST(UniformVariableTest, DefaultConstructor) {
  UniformVariable u;
  EXPECT_TRUE(u.empty());
  EXPECT_EQ(u, u);
}

TEST(UniformVariableTest, ValueConstructor) {
  UniformVariable u(5.0f);
  EXPECT_EQ(u.getType(), render::UniformType::Float);
  EXPECT_EQ(u.get<float>(), 5.0f);

  // Invalid type
  EXPECT_THROW(u.get<int>(), core::Exception);

  // Set new value
  u.set(math::vec3(1, 1, 1));
  EXPECT_EQ(u.getType(), render::UniformType::Float3);
  EXPECT_EQ(u.get<math::vec3>(), math::vec3(1, 1, 1));
}

TEST(UniformVariableTest, Comparison) {
  UniformVariable u1(5.0f);
  UniformVariable u2(5.1f);
  UniformVariable u3(5);

  EXPECT_EQ(u1, u1);
  EXPECT_NE(u1, u2);
  EXPECT_NE(u1, u3);
}

TEST(UniformVariableTest, CopyConstructor) {
  UniformVariable u(5.0f);
  UniformVariable uCopy(u);
  EXPECT_EQ(u, uCopy);

  u = math::vec3(0);
  EXPECT_NE(u, uCopy);

  uCopy = u;
  UniformVariable uMove;
  uMove = std::move(u);
  EXPECT_EQ(u, uMove);
}

} // anonymous namespace
