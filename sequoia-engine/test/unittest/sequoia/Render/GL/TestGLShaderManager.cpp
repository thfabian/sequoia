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
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

class GLShaderManagerTest : public GLRenderTest {};

TEST_F(GLShaderManagerTest, LoadingSuccess) {
  RenderSystem& rsys = RenderSystem::getSingleton();

  Shader* shader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      resolveRessourcePath("sequoia/Render/GL/TestGLShaderManager/VertexCompileSuccess.vert"));

  // Create from source
  GLShader* glShader = dyn_cast<GLShader>(shader);

  EXPECT_TRUE(glShader->isValid());
  EXPECT_NE(glShader->getID(), 0);
  EXPECT_EQ(glShader->getType(), Shader::ST_Vertex);
  EXPECT_EQ(
      glShader->getSourcePath(),
      resolveRessourcePath("sequoia/Render/GL/TestGLShaderManager/VertexCompileSuccess.vert"));

  // Use already existing shader
  Shader* newShader = rsys.loadShader(
      getWindow(), Shader::ST_Vertex,
      resolveRessourcePath("sequoia/Render/GL/TestGLShaderManager/VertexCompileSuccess.vert"));
  EXPECT_EQ(shader, newShader);
  EXPECT_EQ(glShader->getID(), shader->getID());

  // Destroy shader
  rsys.destroyShader(getWindow(), shader);
  EXPECT_FALSE(glShader->isValid());
  EXPECT_EQ(glShader->getID(), 0);
  EXPECT_EQ(shader->getID(), 0);
}

TEST_F(GLShaderManagerTest, LoadingFail) {
  RenderSystem& rsys = RenderSystem::getSingleton();

  // Shader does not exists
  EXPECT_THROW(rsys.loadShader(
                   getWindow(), Shader::ST_Vertex,
                   resolveRessourcePath(
                       "sequoia/Render/GL/TestGLShaderManager/VertexCompileFail-NonExising.vert")),
               RenderSystemException);

  // Shader is invalid
  EXPECT_THROW(rsys.loadShader(getWindow(), Shader::ST_Vertex,
                               resolveRessourcePath(
                                   "sequoia/Render/GL/TestGLShaderManager/VertexCompileFail.vert")),
               RenderSystemException);
}

TEST_F(GLShaderManagerTest, LoadingPartial) {
  GLShaderManager* manager = dyn_cast<GLRenderSystem>(RenderSystem::getSingletonPtr())
                                 ->getRenderer(getWindow())
                                 ->getShaderManager();

  GLShader* shader = manager->create(
      Shader::ST_Vertex,
      resolveRessourcePath("sequoia/Render/GL/TestGLShaderManager/VertexCompileSuccess.vert"),
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
