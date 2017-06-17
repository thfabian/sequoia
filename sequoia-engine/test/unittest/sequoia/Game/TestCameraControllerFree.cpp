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
#include "sequoia/Game/CameraControllerFree.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Unittest/GameTest.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

class CameraControllerFreeTest : public GameTest {};

TEST_F(CameraControllerFreeTest, CameraControllerFree) {
  auto node = SceneGraph::create<CameraControllerFree>("CameraControllerFree");

  EXPECT_STREQ(node->getName().data(), "CameraControllerFree");
  EXPECT_EQ(node->getKind(), SceneNode::SK_CameraControllerFree);
  EXPECT_FALSE(node->hasCamera());

  // Clone node
  auto nodeClone = node->clone();
  nodeClone->setName("copy");
  EXPECT_EQ(nodeClone->getName(), "copy");
  EXPECT_EQ(node->getName(), "CameraControllerFree");

  // Test RTTI
  EXPECT_TRUE(isa<CameraController>(node.get()));
  EXPECT_TRUE(isa<CameraControllerFree>(node.get()));
}

} // anonymous namespace
