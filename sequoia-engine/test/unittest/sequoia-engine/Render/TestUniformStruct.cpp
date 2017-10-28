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

#include "sequoia-engine/Render/UniformStruct.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;

namespace {

SEQUOIA_UNIFORM_STRUCT(ScalarStruct, (float, a)(int, b)(bool, c));

TEST(UniformStructTest, ScalarStruct) {
  ScalarStruct s{4.1f, 2, false};
  EXPECT_EQ(s.a, 4.1f);
  EXPECT_EQ(s.b, 2);
  EXPECT_EQ(s.c, false);
}

SEQUOIA_UNIFORM_STRUCT(VectorStruct, (math::vec3, a)(math::mat4, b));

TEST(UniformStructTest, VectorStruct) {
  VectorStruct v{math::vec3(1), math::mat4(2)};
  EXPECT_EQ(v.a, math::vec3(1));
  EXPECT_EQ(v.b, math::mat4(2));
}

TEST(UniformStructTest, ToUniformVariable) {
  ScalarStruct s{4.1f, 2, false};
  VectorStruct v{math::vec3(1), math::mat4(2)};

  std::unordered_map<std::string, UniformVariable> map;

  s.toUniformVariableMap("ScalarStruct", map, 0);
  ASSERT_TRUE(map.count("ScalarStruct[0].a"));
  ASSERT_EQ(map["ScalarStruct[0].a"], 4.1f);

  ASSERT_TRUE(map.count("ScalarStruct[0].b"));
  ASSERT_EQ(map["ScalarStruct[0].b"], 2);

  ASSERT_TRUE(map.count("ScalarStruct[0].c"));
  ASSERT_EQ(map["ScalarStruct[0].c"], false);

  v.toUniformVariableMap("VectorStruct", map, -1);
  ASSERT_TRUE(map.count("VectorStruct.a"));
  ASSERT_EQ(map["VectorStruct.a"], math::vec3(1));

  ASSERT_TRUE(map.count("VectorStruct.b"));
  ASSERT_EQ(map["VectorStruct.b"], math::mat4(2));
}

} // anonymous namespace
