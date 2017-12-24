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

#include "sequoia-engine/Core/Casting.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/GL/GLRenderer.h"
#include "sequoia-engine/Render/GL/GLShaderManager.h"
#include "sequoia-engine/Unittest/RenderSetup.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include "sequoia-engine/Unittest/TestFile.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::core;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

static std::shared_ptr<Shader> makeShader(Shader::ShaderType type, const char* filename) {
  return RenderSystem::getSingleton().createShader(
      type, filename, TestEnvironment::getSingleton().getFile(filename)->getDataAsString());
}

SEQUOIA_TESTCASEFIXTURE(GLShaderTest, RenderSetup);

TEST_F(GLShaderTest, LoadingSuccess) {
  std::shared_ptr<Shader> shader = makeShader(
      Shader::ST_Vertex, "sequoia-engine/Render/GL/TestGLShader/VertexCompileSuccess.vert");

  // Create from source
  GLShader* glshader = dyn_cast<GLShader>(shader.get());

  EXPECT_TRUE(glshader->isValid());
  EXPECT_NE(glshader->getID(), 0);
  EXPECT_EQ(glshader->getType(), Shader::ST_Vertex);
  EXPECT_STREQ(glshader->getFilename().c_str(),
               "sequoia-engine/Render/GL/TestGLShader/VertexCompileSuccess.vert");

  // Use already existing shader
  std::shared_ptr<Shader> newShader = makeShader(
      Shader::ST_Vertex, "sequoia-engine/Render/GL/TestGLShader/VertexCompileSuccess.vert");

  GLShader* glnewshader = dyn_cast<GLShader>(newShader.get());
  EXPECT_EQ(glshader, glnewshader);
  EXPECT_EQ(glshader->getID(), glnewshader->getID());
}

TEST_F(GLShaderTest, LoadingFail) {
  // Shader does not exists
  EXPECT_THROW(
      makeShader(Shader::ST_Vertex,
                 "sequoia-engine/Render/GL/TestGLShader/VertexCompileFail-NonExising.vert"),
      core::Exception);
  
  // Shader is invalid
  EXPECT_THROW(
      makeShader(Shader::ST_Vertex, "sequoia-engine/Render/GL/TestGLShader/VertexCompileFail.vert"),
      RenderSystemException);
}

} // anonymous namespace
