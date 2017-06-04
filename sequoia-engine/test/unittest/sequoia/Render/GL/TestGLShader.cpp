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
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Unittest/Environment.h"
#include "sequoia/Unittest/GL/GLRenderTest.h"
#include "sequoia/Unittest/TestFile.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::core;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

class GLShaderTest : public GLRenderTest {};

TEST_F(GLShaderTest, LoadingSuccess) {
  Environment& env = Environment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> shader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      env.getFile("sequoia/Render/GL/TestGLShaderManager/VertexCompileSuccess.vert"));

  // Create from source
  GLShader* glshader = dyn_cast<GLShader>(shader.get());

  EXPECT_TRUE(glshader->isValid());
  EXPECT_NE(glshader->getID(), 0);
  EXPECT_EQ(glshader->getType(), Shader::ST_Vertex);
  EXPECT_EQ(*glshader->getFile(),
            *env.getFile("sequoia/Render/GL/TestGLShaderManager/VertexCompileSuccess.vert"));

  // Use already existing shader
  std::shared_ptr<Shader> newShader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      env.getFile("sequoia/Render/GL/TestGLShaderManager/VertexCompileSuccess.vert"));

  GLShader* glnewshader = dyn_cast<GLShader>(newShader.get());
  EXPECT_EQ(glshader, glnewshader);
  EXPECT_EQ(glshader->getID(), glnewshader->getID());
}

TEST_F(GLShaderTest, LoadingFail) {
  Environment& env = Environment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  // Shader does not exists
  EXPECT_THROW(
      rsys.loadShader(
          getWindow(), Shader::ST_Vertex,
          env.getFile("sequoia/Render/GL/TestGLShaderManager/VertexCompileFail-NonExising.vert")),
      Exception);

  // Shader is invalid
  EXPECT_THROW(
      rsys.loadShader(getWindow(), Shader::ST_Vertex,
                      env.getFile("sequoia/Render/GL/TestGLShaderManager/VertexCompileFail.vert")),
      RenderSystemException);
}

TEST_F(GLShaderTest, LoadingPartial) {
  Environment& env = Environment::getSingleton();

  GLShaderManager* manager = dyn_cast<GLRenderSystem>(RenderSystem::getSingletonPtr())
                                 ->getRenderer(getWindow())
                                 ->getShaderManager();

  std::shared_ptr<GLShader> shader = manager->create(
      Shader::ST_Vertex,
      env.getFile("sequoia/Render/GL/TestGLShaderManager/VertexCompileSuccess.vert"),
      GLShaderStatus::OnDisk);

  // OnDisk
  EXPECT_EQ(shader->getStatus(), GLShaderStatus::OnDisk);
  EXPECT_FALSE(shader->isValid());
  EXPECT_EQ(shader->getID(), 0);

  // InMemory
  manager->make(shader, GLShaderStatus::InMemory);
  EXPECT_EQ(shader->getStatus(), GLShaderStatus::InMemory);
  EXPECT_FALSE(shader->isValid());
  EXPECT_EQ(shader->getID(), 0);
  EXPECT_FALSE(shader->getSourceCode().empty());

  // Created
  manager->make(shader, GLShaderStatus::Created);
  EXPECT_EQ(shader->getStatus(), GLShaderStatus::Created);
  EXPECT_FALSE(shader->isValid());
  EXPECT_NE(shader->getID(), 0);
  EXPECT_FALSE(shader->getSourceCode().empty());

  // Compiled
  manager->makeValid(shader);
  EXPECT_EQ(shader->getStatus(), GLShaderStatus::Compiled);
  EXPECT_TRUE(shader->isValid());
  EXPECT_NE(shader->getID(), 0);
  EXPECT_FALSE(shader->getSourceCode().empty());
}

} // anonymous namespace
