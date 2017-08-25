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

#include "sequoia/Render/GlobalRenderState.h"
#include "sequoia/Render/RenderSystem.h"
#include "sequoia/Unittest/TestEnvironment.h"
#include "sequoia/Unittest/TestOptions.h"
#include <gtest/gtest.h>

using namespace sequoia::render;
using namespace sequoia::unittest;

namespace {

TEST(GlobalRenderStateTest, UniformVariables) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto ptr = RenderSystem::create(RK_Null, TestOptions::getSingletonPtr());

  auto shader1 = ptr->createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/TestGlobalRenderState/Shader1.vert"));
  auto shader2 = ptr->createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/TestGlobalRenderState/Shader2.vert"));

  auto program1 = ptr->createProgram({shader1});
  auto program2 = ptr->createProgram({shader2});

  GlobalRenderState gstate;
  EXPECT_FALSE(gstate.hasUniformVariables(program1));
  EXPECT_FALSE(gstate.hasUniformVariables(program2));

  gstate.setUniformVariable(program1, "int2", UniformVariable(int(2)));
  EXPECT_TRUE(gstate.hasUniformVariables(program1));
  EXPECT_FALSE(gstate.getUniformVariables(program1).empty());

  gstate.setUniformVariable(program1, "float3", UniformVariable(float(3)));
  EXPECT_EQ(gstate.getUniformVariables(program1).size(), 2);

  gstate.setUniformVariable(program2, "bool1", UniformVariable(bool(1)));
  EXPECT_TRUE(gstate.hasUniformVariables(program2));
  EXPECT_FALSE(gstate.getUniformVariables(program2).empty());

  ASSERT_NE(gstate.getUniformVariables(program1).find("float3"),
            gstate.getUniformVariables(program1).end());
  ASSERT_NE(gstate.getUniformVariables(program1).find("int2"),
            gstate.getUniformVariables(program1).end());
  ASSERT_NE(gstate.getUniformVariables(program2).find("bool1"),
            gstate.getUniformVariables(program2).end());

  EXPECT_EQ(gstate.getUniformVariables(program1).find("float3")->second.get<float>(), float(3));
  EXPECT_EQ(gstate.getUniformVariables(program1).find("int2")->second.get<int>(), int(2));
  EXPECT_EQ(gstate.getUniformVariables(program2).find("bool1")->second.get<bool>(), bool(1));
}

} // anonymous namespace
