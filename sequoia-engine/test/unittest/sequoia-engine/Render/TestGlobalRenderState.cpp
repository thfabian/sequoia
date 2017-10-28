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

#include "sequoia-engine/Render/GlobalRenderState.h"
#include "sequoia-engine/Render/RenderSystem.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include "sequoia-engine/Unittest/TestOptions.h"
#include <gtest/gtest.h>

using namespace sequoia::render;
using namespace sequoia::unittest;

namespace {

TEST(GlobalRenderStateTest, PerProgramUniformVariables) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto ptr = RenderSystem::create(RK_Null, TestOptions::getSingletonPtr());

  auto shader1 = ptr->createShader(
      Shader::ST_Vertex, env.getFile("sequoia-engine/Render/TestGlobalRenderState/Shader1.vert"));
  auto shader2 = ptr->createShader(
      Shader::ST_Vertex, env.getFile("sequoia-engine/Render/TestGlobalRenderState/Shader2.vert"));

  auto program1Ptr = ptr->createProgram({shader1});
  auto program1 = program1Ptr.get();

  auto program2Ptr = ptr->createProgram({shader2});
  auto program2 = program2Ptr.get();

  GlobalRenderState gstate;
  EXPECT_FALSE(gstate.hasPerProgramUniformVariables(program1));
  EXPECT_FALSE(gstate.hasPerProgramUniformVariables(program2));

  // No variables registered, should return nullptr
  EXPECT_EQ(gstate.getPerProgramUniformVariables(program1), nullptr);
  EXPECT_EQ(gstate.getPerProgramUniformVariables(program2), nullptr);

  // Set int(2) to program1, size == 1
  gstate.setPerProgramUniformVariable(program1, "int2", UniformVariable(int(2)));
  EXPECT_TRUE(gstate.hasPerProgramUniformVariables(program1));
  EXPECT_EQ(gstate.getPerProgramUniformVariables(program1)->size(), 1);

  // Set float(3) to program1, size == 2
  gstate.setPerProgramUniformVariable(program1, "float3", UniformVariable(float(3)));
  EXPECT_EQ(gstate.getPerProgramUniformVariables(program1)->size(), 2);

  // Set bool(1) to program2, size == 1
  gstate.setPerProgramUniformVariable(program2, "bool1", UniformVariable(bool(1)));
  EXPECT_TRUE(gstate.hasPerProgramUniformVariables(program2));
  EXPECT_FALSE(gstate.getPerProgramUniformVariables(program2)->empty());

  // Check maps contain the values
  ASSERT_NE(gstate.getPerProgramUniformVariables(program1)->find("float3"),
            gstate.getPerProgramUniformVariables(program1)->end());
  ASSERT_NE(gstate.getPerProgramUniformVariables(program1)->find("int2"),
            gstate.getPerProgramUniformVariables(program1)->end());
  ASSERT_NE(gstate.getPerProgramUniformVariables(program2)->find("bool1"),
            gstate.getPerProgramUniformVariables(program2)->end());

  // Check the values
  EXPECT_EQ(gstate.getPerProgramUniformVariables(program1)->find("float3")->second.get<float>(),
            float(3));
  EXPECT_EQ(gstate.getPerProgramUniformVariables(program1)->find("int2")->second.get<int>(),
            int(2));
  EXPECT_EQ(gstate.getPerProgramUniformVariables(program2)->find("bool1")->second.get<bool>(),
            bool(1));

  gstate.reset();
  EXPECT_EQ(gstate.getPerProgramUniformVariables(program1), nullptr);
  EXPECT_EQ(gstate.getPerProgramUniformVariables(program2), nullptr);
}

TEST(GlobalRenderStateTest, SharedUniformVariables) {
  GlobalRenderState gstate;
  gstate.setSharedUniformVariable("int2", UniformVariable(int(2)));

  EXPECT_EQ(gstate.getSharedUniformVariables().size(), 1);
  ASSERT_NE(gstate.getSharedUniformVariables().find("int2"),
            gstate.getSharedUniformVariables().end());
  EXPECT_EQ(gstate.getSharedUniformVariables().find("int2")->second.get<int>(), int(2));

  gstate.reset();
  EXPECT_EQ(gstate.getSharedUniformVariables().size(), 0);
}

} // anonymous namespace
