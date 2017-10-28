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
#include "sequoia-engine/Game/CameraControllerFree.h"
#include "sequoia-engine/Game/MeshManager.h"
#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Game/SceneGraph.h"
#include "sequoia-engine/Unittest/GameSetup.h"
#include "sequoia-engine/Unittest/Test.h"

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

SEQUOIA_TESTCASEFIXTURE(CameraControllerFreeTest, GameSetup);

TEST_F(CameraControllerFreeTest, CameraControllerFree) {
  auto node = SceneNode::allocate<CameraControllerFree>("CameraControllerFree");

  EXPECT_STREQ(node->getName().data(), "CameraControllerFree");
  EXPECT_EQ(node->getKind(), SceneNode::SK_CameraControllerFree);
  EXPECT_FALSE(node->hasCamera());

  auto camera = std::make_shared<render::Camera>(math::vec3(10, 10, 10), math::vec3(-1, 5, 0));
  node->setCamera(camera);
  EXPECT_TRUE(node->hasCamera());

  // Check that the correct orientation (i.e yaw and pitch) was captured
  node->update(SceneNode::UpdateEvent());
  EXPECT_VEC_NEAR(node->getCamera()->getEye(), math::vec3(10, 10, 10), 1e-05f);
  EXPECT_VEC_NEAR(node->getCamera()->getCenter(), math::vec3(-1, 5, 0), 1e-05f);

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
