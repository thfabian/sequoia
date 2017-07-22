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
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Game/SceneNode.h"
#include "sequoia/Unittest/GameSetup.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

SEQUOIA_GAME_TEST_FIXTURE(SceneNodeTest);

TEST_F(SceneNodeTest, Construction) {
  auto node = SceneNode::allocate("SceneNode");

  EXPECT_STREQ(node->getName().data(), "SceneNode");
  EXPECT_EQ(node->getKind(), SceneNode::SK_SceneNode);
  EXPECT_FALSE(node->hasChildren());
  EXPECT_FALSE(node->hasParent());

  // Adjust scale
  node->setScale(2.0f);
  EXPECT_EQ(node->getScale(), 2.0f);
  EXPECT_EQ(node->getModelMatrix()[0][0], 2.0f);
  EXPECT_EQ(node->getModelMatrix()[1][1], 2.0f);
  EXPECT_EQ(node->getModelMatrix()[2][2], 2.0f);

  // Adjust position
  node->setPosition(math::vec3(1.0f, 2.0f, 3.0f));
  EXPECT_EQ(node->getPosition(), math::vec3(1.0f, 2.0f, 3.0f));
  EXPECT_EQ(node->getModelMatrix()[3][0], 1.0f);
  EXPECT_EQ(node->getModelMatrix()[3][1], 2.0f);
  EXPECT_EQ(node->getModelMatrix()[3][2], 3.0f);

  // Adjust orientation
  node->setOrientation(math::rotation(math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, -1.0f, 0.0f)));
  EXPECT_EQ(node->getModelMatrix()[1][1], -2.0f);
  EXPECT_EQ(node->getModelMatrix()[2][2], -2.0f);

  // Clone node
  auto nodeClone = node->clone();
  nodeClone->setScale(3.0f);
  EXPECT_EQ(nodeClone->getScale(), 3.0f);
  EXPECT_EQ(node->getScale(), 2.0f);

  // Test children
  node->addChild(nodeClone);
  EXPECT_EQ(node->getChildren()[0], nodeClone);
  EXPECT_TRUE(node->hasChildren());
  EXPECT_TRUE(nodeClone->hasParent());
  EXPECT_EQ(nodeClone->getParent(), node);

  // Test RTTI
  EXPECT_TRUE(isa<SceneNode>(node.get()));
}

TEST_F(SceneNodeTest, ApplySequential) {
  auto node = SceneNode::allocate("Parent");
  node->addChild(SceneNode::allocate("Child"));

  int numNodes;

  // Normal execution
  numNodes = 0;
  node->apply([&numNodes](SceneNode*) { numNodes += 1; }, SceneNode::EP_Sequential);
  EXPECT_EQ(numNodes, 2);

  // Noexcept execution
  numNodes = 0;
  node->apply([&numNodes](SceneNode*) noexcept { numNodes += 1; }, SceneNode::EP_Sequential);
  EXPECT_EQ(numNodes, 2);

  // Check exceptions
  EXPECT_THROW(
      node->apply([](SceneNode*) { throw std::runtime_error("test"); }, SceneNode::EP_Sequential),
      std::runtime_error);
}

} // anonymous namespace
