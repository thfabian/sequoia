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

#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Unittest/GameTest.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

class SceneGraphTest : public GameTest {};

TEST_F(SceneGraphTest, Construction) {
  std::shared_ptr<SceneGraph> graph = std::make_shared<SceneGraph>();

  graph->insert(SceneGraph::create<SceneNode>("TestNode"));
  EXPECT_EQ(graph->size(), 1);
}

} // anonymous namespace