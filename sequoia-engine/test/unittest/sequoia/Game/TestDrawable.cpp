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
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Game/Drawable.h"
#include "sequoia/Unittest/GameTest.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

class DrawableTest : public GameTest {};

TEST_F(DrawableTest, Construction) {
  Game& game = Game::getSingleton();
  auto node = SceneGraph::create<Drawable>("Drawable");

  EXPECT_STREQ(node->getName().data(), "Drawable");
  EXPECT_EQ(node->getKind(), SceneNode::SK_Drawable);
  EXPECT_FALSE(node->hasMesh());

  // Set a mesh
  node->setMesh(game.getMeshManager()->createCube("TestCube"));
  EXPECT_TRUE(node->hasMesh());

  // Clone node
  auto nodeClone = node->clone();
  nodeClone->setName("copy");
  EXPECT_EQ(nodeClone->getName(), "copy");
  EXPECT_EQ(node->getName(), "Drawable");

  // Test RTTI
  EXPECT_TRUE(isa<Drawable>(node.get()));
}

} // anonymous namespace
