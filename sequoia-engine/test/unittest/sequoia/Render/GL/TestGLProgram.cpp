//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Casting.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Unittest/Environment.h"
#include "sequoia/Unittest/GL/GLRenderTest.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

class GLProgramTest : public GLRenderTest {};

TEST_F(GLProgramTest, LinkingSuccess) {
  RenderSystem& rsys = RenderSystem::getSingleton();

  Shader* vertexShader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      resolveRessourcePath("sequoia/Render/GL/TestGLProgramManager/VertexLinkSuccess.vert"));

  Shader* fragmentShader = rsys.loadShader(
      getWindow(), Shader::ST_Fragment,
      resolveRessourcePath("sequoia/Render/GL/TestGLProgramManager/FragmentLinkSuccess.frag"));

  Program* program = rsys.createProgram(getWindow(), {vertexShader, fragmentShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program);

  EXPECT_TRUE(glprogram->isValid());
  EXPECT_NE(glprogram->getID(), 0);
  EXPECT_EQ(glprogram->getShaders(), (std::set<Shader*>{vertexShader, fragmentShader}));

  // Use program
  glprogram->bind();

  GLint id;
  glGetIntegerv(GL_CURRENT_PROGRAM, &id);
  EXPECT_EQ(id, glprogram->getID());

  // Remove shader
  EXPECT_TRUE(glprogram->removeShader(fragmentShader));
  EXPECT_FALSE(glprogram->isValid());
  EXPECT_EQ(glprogram->getStatus(), GLProgramStatus::Created);

  // Destroy program
  rsys.destroyProgram(getWindow(), glprogram);
  EXPECT_FALSE(glprogram->isValid());
  EXPECT_EQ(glprogram->getStatus(), GLProgramStatus::Invalid);
}

TEST_F(GLProgramTest, UniformScalars) {
  RenderSystem& rsys = RenderSystem::getSingleton();

  Shader* vertexShader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      resolveRessourcePath("sequoia/Render/GL/TestGLProgramManager/VertexUniformScalars.vert"));

  Program* program = rsys.createProgram(getWindow(), {vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program);

  EXPECT_EQ(glprogram->getUniformVariables().size(), 2);

  float u_FloatTest = 5.0f;
  EXPECT_TRUE(glprogram->setUniformVariable("u_FloatTest", u_FloatTest));

  int u_IntTest = 5;
  EXPECT_TRUE(glprogram->setUniformVariable("u_IntTest", u_IntTest));

  EXPECT_TRUE(glprogram->checkUniformVariables());
}

TEST_F(GLProgramTest, UniformVectors) {
  RenderSystem& rsys = RenderSystem::getSingleton();

  Shader* vertexShader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      resolveRessourcePath("sequoia/Render/GL/TestGLProgramManager/VertexUniformVectors.vert"));

  Program* program = rsys.createProgram(getWindow(), {vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program);

  EXPECT_EQ(glprogram->getUniformVariables().size(), 3);

  math::vec2 u_fvec2;
  EXPECT_TRUE(glprogram->setUniformVariable("u_fvec2", u_fvec2));

  math::vec3 u_fvec3;
  EXPECT_TRUE(glprogram->setUniformVariable("u_fvec3", u_fvec3));
  EXPECT_THROW(glprogram->setUniformVariable("u_fvec2", u_fvec3), render::RenderSystemException);

  math::vec4 u_fvec4;
  EXPECT_TRUE(glprogram->setUniformVariable("u_fvec4", u_fvec4));

  EXPECT_TRUE(glprogram->checkUniformVariables());
}

TEST_F(GLProgramTest, UniformMatrices) {
  RenderSystem& rsys = RenderSystem::getSingleton();

  Shader* vertexShader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      resolveRessourcePath("sequoia/Render/GL/TestGLProgramManager/VertexUniformMatrices.vert"));

  Program* program = rsys.createProgram(getWindow(), {vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program);

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
  RenderSystem& rsys = RenderSystem::getSingleton();

  Shader* vertexShader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      resolveRessourcePath("sequoia/Render/GL/TestGLProgramManager/VertexAttributesAll.vert"));

  Program* program = rsys.createProgram(getWindow(), {vertexShader});
  GLProgram* glprogram = dyn_cast<GLProgram>(program);

  // Check the locations
  GLVertexAttribute::forEach([&glprogram](unsigned int index, const char* name) {
    GLint location = glGetAttribLocation(glprogram->getID(), name);
    EXPECT_EQ(location, index) << "invalid location for attribute: \"" << name << "\"";
  });
}

TEST_F(GLProgramTest, VertexAttributesFail) {
  RenderSystem& rsys = RenderSystem::getSingleton();

  Shader* vertexShader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      resolveRessourcePath("sequoia/Render/GL/TestGLProgramManager/VertexAttributesFail.vert"));

  EXPECT_THROW((rsys.createProgram(getWindow(), {vertexShader})), RenderSystemException);
}

} // anonymous namespace
