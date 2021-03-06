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

#include "sequoia-engine/Game/Shape.h"
#include "sequoia-engine/Game/ShapeManager.h"
#include "sequoia-engine/Render/VertexData.h"
#include "sequoia-engine/Unittest/GameSetup.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

SEQUOIA_TESTCASEFIXTURE(ShapeManagerTest, GameSetup);

TEST_F(ShapeManagerTest, Cube) {
  Game& game = Game::getSingleton();

  std::shared_ptr<Shape> shape = game.getShapeManager()->createCube("TestCube");

  ASSERT_EQ(shape->getMeshes().size(), 1);
  ASSERT_EQ(shape->getMaterials().size(), 1);

  EXPECT_EQ(shape->getMeshes().front()->getVertexData()->getNumIndices(), 36);
  EXPECT_EQ(shape->getMeshes().front()->getVertexData()->getNumVertices(), 24);
  EXPECT_FALSE(shape->getMeshes().front()->isModifiable());
  EXPECT_TRUE(shape->getMeshes().front()->getAxisAlignedBox() ==
              math::AxisAlignedBox(math::vec3(-0.5, -0.5, -0.5), math::vec3(0.5, 0.5, 0.5)));
}

//TEST_F(ShapeManagerTest, Obj) {
//  Game& game = Game::getSingleton();
//  TestEnvironment& env = TestEnvironment::getSingleton();

//  std::shared_ptr<Mesh> mesh = game.getShapeManager()->load("TestCube", env.getFile("sequoia-engine/Game/TestMeshManager/Cube.obj"));

////  EXPECT_EQ(mesh->getVertexData()->getNumIndices(), 36);
////  EXPECT_EQ(mesh->getVertexData()->getNumVertices(), 24);
////  EXPECT_FALSE(mesh->isModifiable());
////  EXPECT_TRUE(mesh->getAxisAlignedBox() ==
////              math::AxisAlignedBox(math::vec3(0, 0, 0), math::vec3(2, 2, 2)));
//}

//TEST_F(ShapeManagerTest, FreeUnusedMeshes) {
//  // TODO: We currently load a default scene with a cube so we can't yet test this
//}

} // anonymous namespace
