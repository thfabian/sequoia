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
#include "sequoia/Game/Drawable.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Unittest/GameSetup.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

SEQUOIA_GAME_TEST_FIXTURE(DrawableTest);

TEST_F(DrawableTest, Construction) {
  Game& game = Game::getSingleton();
  std::shared_ptr<SceneNode> node = SceneNode::allocate("TestNode");
  auto mesh = game.getMeshManager()->createCube("TestCube");

  node->addCapability<Drawable>(mesh);
  EXPECT_TRUE(node->is<Drawable>());
  EXPECT_TRUE(node->get<Drawable>()->hasMesh());

  // Clone node
  auto nodeClone = node->clone();
  nodeClone->setName("copy");
  EXPECT_EQ(nodeClone->getName(), "copy");
  EXPECT_EQ(node->getName(), "TestNode");

  EXPECT_TRUE(nodeClone->is<Drawable>());
  EXPECT_TRUE(nodeClone->get<Drawable>()->hasMesh());

  // Check Drawable was cloned (compare addresses)
  EXPECT_NE((std::size_t)node->get<Drawable>(), (std::size_t)nodeClone->get<Drawable>());

  // Type info
  static_assert(SceneNodeCapabilityToKind<Drawable>::value == SceneNodeCapability::Drawable,
                "invalid type");
  static_assert(
      std::is_same<SceneNodeCapabilityToType<SceneNodeCapability::Drawable>::type, Drawable>::value,
      "invalid kind");
}

} // anonymous namespace
