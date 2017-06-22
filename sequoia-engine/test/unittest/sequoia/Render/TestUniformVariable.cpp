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

#include "sequoia/Core/MicroBenchmark.h"
#include "sequoia/Render/UniformVariable.h"
#include "sequoia/Unittest/Environment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

// Benchmark boost::any vs. boost::variant for assigning data
TEST(UniformVariableTest, AnyVsVariantAssign) {
  SEQUOIA_BENCHMARK_TEST;

  boost::any any;
  UniformVariable::DataType variant;

  for(std::size_t i = 0; i < (1 << 14); ++i) {
    MIB_START("boost::any assign mat4");
    any = math::mat4(2.0f);
    MIB_NEXT("boost::variant assign mat4");
    variant = math::mat4(2.0f);
    MIB_NEXT("boost::any assign int");
    any = int(4);
    MIB_NEXT("boost::variant assign int");
    variant = int(4);
    MIB_STOP("boost::variant assign int");
  }

  MIB_PRINT("cycle");
}

// Benchmark boost::any vs. boost::variant copy constructing
TEST(UniformVariableTest, AnyVsVariantCopy) {
  SEQUOIA_BENCHMARK_TEST;

  boost::any any;
  UniformVariable::DataType variant;

  for(std::size_t i = 0; i < (1 << 14); ++i) {
    {
      any = math::mat4(2.0f);
      MIB_START("boost::any copy mat4");
      boost::any copy(any);
      MIB_STOP("boost::any copy mat4");
      EXPECT_FALSE(copy.empty());
    }

    {
      variant = math::mat4(2.0f);
      MIB_START("boost::variant copy mat4");
      UniformVariable::DataType copy(variant);
      MIB_STOP("boost::variant copy mat4");
      EXPECT_FALSE(copy.empty());
    }

    {
      any = int(4);
      MIB_START("boost::any copy int");
      boost::any copy(any);
      MIB_STOP("boost::any copy int");
      EXPECT_FALSE(copy.empty());
    }

    {
      variant = int(4);
      MIB_START("boost::variant copy int");
      UniformVariable::DataType copy(variant);
      MIB_STOP("boost::variant copy int");
      EXPECT_FALSE(copy.empty());
    }
  }

  MIB_PRINT("cycle");
}

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
