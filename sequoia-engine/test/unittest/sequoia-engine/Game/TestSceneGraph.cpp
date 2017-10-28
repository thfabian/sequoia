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

#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Game/SceneGraph.h"
#include "sequoia-engine/Unittest/GameSetup.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

SEQUOIA_TESTCASEFIXTURE(SceneGraphTest, GameSetup);

TEST_F(SceneGraphTest, Insert) {
  std::shared_ptr<SceneGraph> graph = std::make_shared<SceneGraph>();

  graph->insert(SceneNode::allocate<SceneNode>("TestNode_1"));
  EXPECT_EQ(graph->size(), 1);

  graph->insert(SceneNode::allocate<SceneNode>("TestNode_2"));
  EXPECT_EQ(graph->size(), 2);
}

TEST_F(SceneGraphTest, Remove) {
  std::shared_ptr<SceneGraph> graph = std::make_shared<SceneGraph>();

  graph->insert(SceneNode::allocate<SceneNode>("TestNode_1"));
  graph->insert(SceneNode::allocate<SceneNode>("TestNode_2"));

  auto node = SceneNode::allocate<SceneNode>("TestNode_3");
  graph->insert(node);
  EXPECT_EQ(graph->size(), 3);

  graph->remove(node);
  EXPECT_EQ(graph->size(), 2);

  graph->clear();
  EXPECT_EQ(graph->size(), 0);
}

} // anonymous namespace
