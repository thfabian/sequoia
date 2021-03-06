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
#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/GL/GLTextureManager.h"
#include "sequoia-engine/Unittest/RenderSetup.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gli/gli.hpp>
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

SEQUOIA_TESTCASEFIXTURE(GLTextureTest, RenderSetup);

TEST_F(GLTextureTest, LoadTexture2D) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  auto image = Image::load(env.getFile("sequoia-engine/Render/GL/TestGLTexture/Test.png"));
  std::shared_ptr<Texture> texture = rsys.createTexture(image);

  GLTexture* gltexture = core::dyn_cast<GLTexture>(texture.get());
  EXPECT_TRUE(gltexture->isValid());
  EXPECT_NE(gltexture->getID(), 0);

  // Bind texture to texture unit 0
  glActiveTexture(GL_TEXTURE0);
  gltexture->bind();

  GLint id;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);
  EXPECT_EQ(id, gltexture->getID());

  // Bind texture to texture unit 5
  glActiveTexture(GL_TEXTURE5);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &id); // No texture should be bound to this unit
  EXPECT_EQ(id, 0);

  gltexture->bind(); // Bind our texture to 5
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);
  EXPECT_EQ(id, gltexture->getID());

  // Reload texture with the same parameters -> should return the same texture
  std::shared_ptr<Texture> sameTexture = rsys.createTexture(image);

  GLTexture* glsameTexture = core::dyn_cast<GLTexture>(sameTexture.get());
  EXPECT_EQ(gltexture->getID(), glsameTexture->getID());

  // Reload texture with the different parameters -> should return a new texture
  TextureParameter param;
  param.UseMipmap = false;
  std::shared_ptr<Texture> newTexture = rsys.createTexture(image, param);

  GLTexture* glnewTexture = core::dyn_cast<GLTexture>(newTexture.get());
  EXPECT_NE(gltexture->getID(), glnewTexture->getID());
}

TEST_F(GLTextureTest, LoadTexture2DCompressed) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  auto image = Image::load(env.getFile("sequoia-engine/Render/GL/TestGLTexture/Test.dds"));
  std::shared_ptr<Texture> texture = rsys.createTexture(image);

  GLTexture* gltexture = core::dyn_cast<GLTexture>(texture.get());
  ASSERT_TRUE(core::isa<TextureImage>(image.get()));
  const gli::texture& gliTexture = core::dyn_cast<TextureImage>(image.get())->getTexture();

  // Base level
  {
    GLint width, height;
    glGetTextureLevelParameteriv(gltexture->getID(), 0, GL_TEXTURE_WIDTH, &width);
    glGetTextureLevelParameteriv(gltexture->getID(), 0, GL_TEXTURE_HEIGHT, &height);

    EXPECT_EQ(width, gliTexture.extent().x);
    EXPECT_EQ(height, gliTexture.extent().y);

    GLint compressed;
    glGetTextureLevelParameteriv(gltexture->getID(), 0, GL_TEXTURE_COMPRESSED, &compressed);
    EXPECT_EQ((bool)compressed, gli::is_compressed(gliTexture.format()));
  }

  // First level of detail (i.e first mip-map level)
  {
    GLint width, height;
    glGetTextureLevelParameteriv(gltexture->getID(), 1, GL_TEXTURE_WIDTH, &width);
    glGetTextureLevelParameteriv(gltexture->getID(), 1, GL_TEXTURE_HEIGHT, &height);

    EXPECT_EQ(width, gliTexture.extent(1).x);
    EXPECT_EQ(height, gliTexture.extent(1).y);
  }
}

TEST_F(GLTextureTest, RTTI) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  RenderSystem& rsys = RenderSystem::getSingleton();

  auto image = Image::load(env.getFile("sequoia-engine/Render/GL/TestGLTexture/Test.png"));
  std::shared_ptr<Texture> texture = rsys.createTexture(image);
  EXPECT_TRUE(core::isa<GLTexture>(texture.get()));
}

} // anonymous namespace
