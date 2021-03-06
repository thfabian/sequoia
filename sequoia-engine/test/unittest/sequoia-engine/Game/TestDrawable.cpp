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
#include "sequoia-engine/Game/Drawable.h"
#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Game/SceneGraph.h"
#include "sequoia-engine/Game/ShapeManager.h"
#include "sequoia-engine/Unittest/GameSetup.h"
#include <gtest/gtest.h>

using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

SEQUOIA_TESTCASEFIXTURE(DrawableTest, GameSetup);

TEST_F(DrawableTest, Construction) {
  Game& game = Game::getSingleton();
  std::shared_ptr<SceneNode> node = SceneNode::allocate("TestNode");
  auto mesh = game.getShapeManager()->createCube("TestCube");

  node->addCapability<Drawable>(mesh);
  EXPECT_TRUE(node->is<Drawable>());

  // Clone node
  auto nodeClone = node->clone();
  nodeClone->setName("copy");
  EXPECT_EQ(nodeClone->getName(), "copy");
  EXPECT_EQ(node->getName(), "TestNode");

  EXPECT_TRUE(nodeClone->is<Drawable>());

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
