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

#include "sequoia/Game/MeshManager.h"
#include "sequoia/Unittest/GameSetup.h"
#include "sequoia/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

class MeshManagerTest : public TestFixture<GameSetup> {};

TEST_F(MeshManagerTest, Cube) {
  Game& game = Game::getSingleton();

  std::shared_ptr<Mesh> mesh = game.getMeshManager()->createCube("TestCube");

  EXPECT_EQ(mesh->getVertexData()->getNumIndices(), 36);
  EXPECT_EQ(mesh->getVertexData()->getNumVertices(), 24);
  EXPECT_FALSE(mesh->isModifiable());
  EXPECT_TRUE(mesh->getAxisAlignedBox() ==
              math::AxisAlignedBox(math::vec3(-0.5, -0.5, -0.5), math::vec3(0.5, 0.5, 0.5)));
}

TEST_F(MeshManagerTest, Obj) {
  Game& game = Game::getSingleton();
  TestEnvironment& env = TestEnvironment::getSingleton();

  std::shared_ptr<Mesh> mesh =
      game.getMeshManager()->load("TestCube", env.getFile("sequoia/Game/TestMeshManager/Cube.obj"),
                                  env.getFile("sequoia/Game/TestMeshManager/Cube.mtl"));

  EXPECT_EQ(mesh->getVertexData()->getNumIndices(), 36);
  EXPECT_EQ(mesh->getVertexData()->getNumVertices(), 24);
  EXPECT_FALSE(mesh->isModifiable());
  EXPECT_TRUE(mesh->getAxisAlignedBox() ==
              math::AxisAlignedBox(math::vec3(0, 0, 0), math::vec3(2, 2, 2)));
}

TEST_F(MeshManagerTest, FreeUnusedMeshes) {
  // TODO: We currently load a default scene with a cube so we can't yet test this
}

} // anonymous namespace
