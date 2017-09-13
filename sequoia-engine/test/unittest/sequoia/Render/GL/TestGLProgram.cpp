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

#include "sequoia/Core/Casting.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Render/UniformStruct.h"
#include "sequoia/Render/UniformVariable.h"
#include "sequoia/Unittest/GL/GLRenderSetup.h"
#include "sequoia/Unittest/TestEnvironment.h"
#include <array>
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

SEQUOIA_TESTCASEFIXTURE(GLProgramTest, GLRenderSetup);

TEST_F(GLProgramTest, LinkingSuccess) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> vertexShader = rsys.createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/GL/TestGLProgram/VertexLinkSuccess.vert"));

  std::shared_ptr<Shader> fragmentShader = rsys.createShader(
      Shader::ST_Fragment, env.getFile("sequoia/Render/GL/TestGLProgram/FragmentLinkSuccess.frag"));

  std::shared_ptr<Program> program = rsys.createProgram({vertexShader, fragmentShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program.get());

  EXPECT_TRUE(glprogram->isValid());
  EXPECT_NE(glprogram->getID(), 0);
  EXPECT_EQ(glprogram->getShaders(),
            (std::set<std::shared_ptr<Shader>>{vertexShader, fragmentShader}));

  // Use program
  glprogram->bind();

  GLint id;
  glGetIntegerv(GL_CURRENT_PROGRAM, &id);
  EXPECT_EQ(id, glprogram->getID());
}

TEST_F(GLProgramTest, UniformScalars) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> vertexShader = rsys.createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/GL/TestGLProgram/VertexUniformScalars.vert"));

  std::shared_ptr<Program> program = rsys.createProgram({vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program.get());

  EXPECT_EQ(glprogram->getUniformVariables().size(), 2);

  float u_FloatTest = 5.0f;
  EXPECT_TRUE(glprogram->setUniformVariable("u_FloatTest", u_FloatTest));

  int u_IntTest = 5;
  EXPECT_TRUE(glprogram->setUniformVariable("u_IntTest", u_IntTest));

  EXPECT_TRUE(glprogram->checkUniformVariables());
}

TEST_F(GLProgramTest, UniformVectors) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> vertexShader = rsys.createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/GL/TestGLProgram/VertexUniformVectors.vert"));

  std::shared_ptr<Program> program = rsys.createProgram({vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program.get());

  EXPECT_EQ(glprogram->getUniformVariables().size(), 3);

  math::vec2 u_fvec2;
  EXPECT_TRUE(glprogram->setUniformVariable("u_fvec2", u_fvec2));

  math::vec3 u_fvec3;
  EXPECT_TRUE(glprogram->setUniformVariable("u_fvec3", u_fvec3));

  // Invalid type -> throw
  EXPECT_THROW(glprogram->setUniformVariable("u_fvec2", u_fvec3), render::RenderSystemException);

  math::vec4 u_fvec4;
  EXPECT_TRUE(glprogram->setUniformVariable("u_fvec4", u_fvec4));

  EXPECT_TRUE(glprogram->checkUniformVariables());
}

TEST_F(GLProgramTest, UniformArrays) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> vertexShader = rsys.createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/GL/TestGLProgram/VertexUniformArrays.vert"));

  std::shared_ptr<Program> program = rsys.createProgram({vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program.get());

  EXPECT_EQ(glprogram->getUniformVariables().size(), 2);

  ::sequoia::render::UniformVariable u_float_array({1.0f, 2.0f});
  EXPECT_TRUE(glprogram->setUniformVariable("u_float_array", u_float_array));

  // Invalid type -> throw
  ::sequoia::render::UniformVariable u_int_array({1, 2});
  EXPECT_THROW(glprogram->setUniformVariable("u_float_array", u_int_array),
               render::RenderSystemException);

  // Invalid size -> throw
  ::sequoia::render::UniformVariable u_float_array_too_big({1.0f, 2.0f, 3.0f});
  EXPECT_THROW(glprogram->setUniformVariable("u_float_array", u_float_array_too_big),
               render::RenderSystemException);

  ::sequoia::render::UniformVariable u_fvec3_array({math::vec3(1), math::vec3(2)});
  EXPECT_TRUE(glprogram->setUniformVariable("u_fvec3_array", u_fvec3_array));
}

SEQUOIA_UNIFORM_STRUCT(ScalarStruct, (int, a)(float, b)(math::vec3, c));

TEST_F(GLProgramTest, UniformStruct) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> vertexShader = rsys.createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/GL/TestGLProgram/VertexUniformStruct.vert"));

  std::shared_ptr<Program> program = rsys.createProgram({vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program.get());

  EXPECT_EQ(glprogram->getUniformVariables().size(), 3);

  ScalarStruct salarStruct{2, 2.1f, math::vec3(2)};

  std::unordered_map<std::string, ::sequoia::render::UniformVariable> map;
  salarStruct.toUniformVariableMap("u_ScalarStruct", map, -1);

  for(const auto& m : map)
    EXPECT_TRUE(glprogram->setUniformVariable(m.first, m.second));
}

TEST_F(GLProgramTest, UniformArrayOfStruct) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> vertexShader = rsys.createShader(
      Shader::ST_Vertex,
      env.getFile("sequoia/Render/GL/TestGLProgram/VertexUniformArrayOfStruct.vert"));

  std::shared_ptr<Program> program = rsys.createProgram({vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program.get());

  EXPECT_EQ(glprogram->getUniformVariables().size(), 9);

  std::array<ScalarStruct, 3> salarStructArray;
  salarStructArray[0] = ScalarStruct{0, 0.1f, math::vec3(0)};
  salarStructArray[1] = ScalarStruct{1, 1.1f, math::vec3(1)};
  salarStructArray[2] = ScalarStruct{2, 2.1f, math::vec3(2)};

  std::unordered_map<std::string, ::sequoia::render::UniformVariable> map;
  salarStructArray[0].toUniformVariableMap("u_ScalarStruct", map, 0);
  salarStructArray[1].toUniformVariableMap("u_ScalarStruct", map, 1);
  salarStructArray[2].toUniformVariableMap("u_ScalarStruct", map, 2);

  for(const auto& m : map)
    EXPECT_TRUE(glprogram->setUniformVariable(m.first, m.second));
}

TEST_F(GLProgramTest, UniformMatrices) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> vertexShader = rsys.createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/GL/TestGLProgram/VertexUniformMatrices.vert"));

  std::shared_ptr<Program> program = rsys.createProgram({vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program.get());

  EXPECT_EQ(glprogram->getUniformVariables().size(), 3);

  math::mat2 u_fmat2;
  EXPECT_TRUE(glprogram->setUniformVariable("u_fmat2", u_fmat2));

  math::mat3 u_fmat3;
  EXPECT_TRUE(glprogram->setUniformVariable("u_fmat3", u_fmat3));

  math::mat4 u_fmat4;
  EXPECT_TRUE(glprogram->setUniformVariable("u_fmat4", u_fmat4));

  EXPECT_TRUE(glprogram->checkUniformVariables());
}

TEST_F(GLProgramTest, VertexAttributesAll) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> vertexShader = rsys.createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/GL/TestGLProgram/VertexAttributesAll.vert"));

  std::shared_ptr<Program> program = rsys.createProgram({vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program.get());

  // Check the locations
  GLVertexAttribute::forEach([&glprogram](unsigned int index, const char* name) {
    GLint location = glGetAttribLocation(glprogram->getID(), name);
    EXPECT_EQ(location, index) << "invalid location for attribute: \"" << name << "\"";
  });
}

TEST_F(GLProgramTest, VertexAttributesFail) {
#if SEQUOIA_ENABLE_ASSERTS
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> vertexShader = rsys.createShader(
      Shader::ST_Vertex, env.getFile("sequoia/Render/GL/TestGLProgram/VertexAttributesFail.vert"));

  EXPECT_THROW((rsys.createProgram({vertexShader})), RenderSystemException);
#endif
}

TEST_F(GLProgramTest, TextureSamplers) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> fragmentShader = rsys.createShader(
      Shader::ST_Fragment, env.getFile("sequoia/Render/GL/TestGLProgram/TextureSamplers.frag"));

  std::shared_ptr<Program> program = rsys.createProgram({fragmentShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program.get());

  EXPECT_TRUE(glprogram->isTextureSampler("tex0_SamplerFor2DTexture"));
  EXPECT_TRUE(glprogram->isTextureSampler("tex5_SamplerFor2DTexture"));

  EXPECT_STREQ(glprogram->getTextureSampler(0).c_str(), "tex0_SamplerFor2DTexture");
  EXPECT_STREQ(glprogram->getTextureSampler(5).c_str(), "tex5_SamplerFor2DTexture");
  EXPECT_TRUE(glprogram->getTextureSampler(1).empty());
}

} // anonymous namespace
