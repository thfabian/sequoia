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
#include "sequoia/Unittest/Environment.h"
#include "sequoia/Unittest/GL/GLRenderTest.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

class GLProgramManagerTest : public GLRenderTest {};

TEST_F(GLProgramManagerTest, LinkingSuccess) {
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
  glprogram->use();

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

} // anonymous namespace
