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
#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Game/SceneGraph.h"
#include "sequoia-engine/Game/SceneNode.h"
#include "sequoia-engine/Unittest/GameSetup.h"
#include <atomic>
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

SEQUOIA_TESTCASEFIXTURE(SceneNodeTest, GameSetup);

TEST_F(SceneNodeTest, Construction) {
  auto node = SceneNode::allocate("SceneNode");

  EXPECT_STREQ(node->getName().data(), "SceneNode");
  EXPECT_EQ(node->getKind(), SceneNode::SK_SceneNode);
  EXPECT_FALSE(node->hasChildren());
  EXPECT_FALSE(node->hasParent());

  // Adjust scale
  node->setScale(2.0f);
  EXPECT_FLOAT_EQ(node->getScale(), 2.0f);
  EXPECT_FLOAT_EQ(node->getModelMatrix()[0][0], 2.0f);
  EXPECT_FLOAT_EQ(node->getModelMatrix()[1][1], 2.0f);
  EXPECT_EQ(node->getModelMatrix()[2][2], 2.0f);

  // Adjust position
  node->setPosition(math::vec3(1.0f, 2.0f, 3.0f));
  EXPECT_EQ(node->getPosition(), math::vec3(1.0f, 2.0f, 3.0f));
  EXPECT_FLOAT_EQ(node->getModelMatrix()[3][0], 1.0f);
  EXPECT_FLOAT_EQ(node->getModelMatrix()[3][1], 2.0f);
  EXPECT_FLOAT_EQ(node->getModelMatrix()[3][2], 3.0f);

  // Adjust orientation
  node->setOrientation(math::rotation(math::vec3(0.0f, 1.0f, 0.0f), math::vec3(0.0f, -1.0f, 0.0f)));
  EXPECT_FLOAT_EQ(node->getModelMatrix()[1][1], -2.0f);
  EXPECT_FLOAT_EQ(node->getModelMatrix()[2][2], -2.0f);

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

TEST_F(SceneNodeTest, Clone) {
  auto node = SceneNode::allocate("Parent");
  node->addChild(SceneNode::allocate("Child"));

  auto clone1 = node->clone();
  EXPECT_STREQ(clone1->getName().c_str(), "Parent_copy_1");
  EXPECT_STREQ(clone1->getChildren().front()->getName().c_str(), "Child_copy_1");

  auto clone2 = clone1->clone();
  EXPECT_STREQ(clone2->getName().c_str(), "Parent_copy_2");
  EXPECT_STREQ(clone2->getChildren().front()->getName().c_str(), "Child_copy_2");
}

TEST_F(SceneNodeTest, ApplySequential) {
  auto node = SceneNode::allocate("Parent");
  node->addChild(SceneNode::allocate("Child"));

  int numNodes;

  // Normal execution
  numNodes = 0;
  node->apply([&numNodes](SceneNode*) { numNodes++; }, SceneNode::EP_Sequential);
  EXPECT_EQ(numNodes, 2);

  // Noexcept execution
  numNodes = 0;
  node->apply([&numNodes](SceneNode*) noexcept { numNodes++; }, SceneNode::EP_Sequential);
  EXPECT_EQ(numNodes, 2);

  // Check exceptions
  EXPECT_THROW(
      node->apply([](SceneNode*) { throw std::runtime_error("test"); }, SceneNode::EP_Sequential),
      std::runtime_error);
}

TEST_F(SceneNodeTest, ApplyParallel) {
  auto node = SceneNode::allocate("Parent");
  node->addChild(SceneNode::allocate("Child"));

  // Normal execution
  std::atomic<int> numNodes(0);
  node->apply([&numNodes](SceneNode*) { numNodes++; }, SceneNode::EP_Parallel);
  EXPECT_EQ(numNodes.load(), 2);

  // Noexcept execution
  numNodes = 0;
  node->apply([&numNodes](SceneNode*) noexcept { numNodes++; }, SceneNode::EP_Parallel);
  EXPECT_EQ(numNodes.load(), 2);
}

} // anonymous namespace
