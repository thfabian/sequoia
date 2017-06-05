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

#include "sequoia/Game/MeshManager.h"
#include "sequoia/Unittest/GameTest.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

class MeshManagerTest : public GameTest {};

TEST_F(MeshManagerTest, Cube) {
  Game& game = Game::getSingleton();

  std::shared_ptr<Mesh> mesh = game.getMeshManager()->createCube("TestCube");

  EXPECT_EQ(mesh->getVertexData()->getNumIndices(), 36);
  EXPECT_EQ(mesh->getVertexData()->getNumVertices(), 24);
  EXPECT_FALSE(mesh->isModifiable());
  EXPECT_TRUE(mesh->getAxisAlignedBox() ==
              math::AxisAlignedBox(math::vec3(-1, -1, -1), math::vec3(1, 1, 1)));
}

} // anonymous namespace
