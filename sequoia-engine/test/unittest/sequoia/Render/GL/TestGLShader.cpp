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
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Unittest/GL/GLRenderSetup.h"
#include "sequoia/Unittest/TestEnvironment.h"
#include "sequoia/Unittest/TestFile.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::core;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

SEQUOIA_TESTCASEFIXTURE(GLShaderTest, GLRenderSetup);

TEST_F(GLShaderTest, LoadingSuccess) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::shared_ptr<Shader> shader =
      rsys.createShader(Shader::ST_Vertex,
                        env.getFile("sequoia/Render/GL/TestGLShader/VertexCompileSuccess.vert"));

  // Create from source
  GLShader* glshader = dyn_cast<GLShader>(shader.get());

  EXPECT_TRUE(glshader->isValid());
  EXPECT_NE(glshader->getID(), 0);
  EXPECT_EQ(glshader->getType(), Shader::ST_Vertex);
  EXPECT_EQ(*glshader->getFile(),
            *env.getFile("sequoia/Render/GL/TestGLShader/VertexCompileSuccess.vert"));

  // Use already existing shader
  std::shared_ptr<Shader> newShader =
      rsys.createShader(Shader::ST_Vertex,
                        env.getFile("sequoia/Render/GL/TestGLShader/VertexCompileSuccess.vert"));

  GLShader* glnewshader = dyn_cast<GLShader>(newShader.get());
  EXPECT_EQ(glshader, glnewshader);
  EXPECT_EQ(glshader->getID(), glnewshader->getID());
}

TEST_F(GLShaderTest, LoadingFail) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  // Shader does not exists
  EXPECT_THROW(rsys.createShader(Shader::ST_Vertex,
                   env.getFile("sequoia/Render/GL/TestGLShader/VertexCompileFail-NonExising.vert")),
               Exception);

  // Shader is invalid
  EXPECT_THROW(
      rsys.createShader(Shader::ST_Vertex,
                        env.getFile("sequoia/Render/GL/TestGLShader/VertexCompileFail.vert")),
      RenderSystemException);
}

} // anonymous namespace
