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
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Game/SceneNode.h"
#include "sequoia/Unittest/GameTest.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

class SceneNodeTest : public GameTest {};

TEST_F(SceneNodeTest, Construction) {
  auto node = SceneGraph::create<SceneNode>("SceneNode");

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

} // anonymous namespace
