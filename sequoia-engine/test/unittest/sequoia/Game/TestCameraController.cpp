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
#include "sequoia/Game/CameraController.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Unittest/GameSetup.h"
#include "sequoia/Unittest/Test.h"

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::game;

namespace {

SEQUOIA_GAME_TEST_FIXTURE(CameraControllerTest);

TEST_F(CameraControllerTest, CameraController) {
  auto node = SceneNode::allocate<CameraController>("CameraController");

  EXPECT_STREQ(node->getName().data(), "CameraController");
  EXPECT_EQ(node->getKind(), SceneNode::SK_CameraController);
  EXPECT_FALSE(node->hasCamera());

  // Create Camera and attach to scene node
  auto camera = std::make_shared<render::Camera>(math::vec3(0, 0, 10), math::vec3(0, 0, 0));
  node->setCamera(camera);
  EXPECT_TRUE(node->hasCamera());

  // Check scene node is positioned correctly
  EXPECT_EQ(node->getPosition(), math::vec3(0, 0, 10));
  EXPECT_VEC_NEAR(node->getOrientation(), math::quat(), 1e-06f);

  // Move the camera and check scene node is positioned correctly
  camera->lookAt(math::vec3(10, 10, 10), math::vec3(0, 0, 0));
  EXPECT_EQ(node->getPosition(), math::vec3(10, 10, 10));
  EXPECT_VEC_NEAR(node->getOrientation(), camera->getOrientation(), 1e-06f);

  // Detach camera
  node->removeCamera();
  EXPECT_FALSE(node->hasCamera());

  node->setPosition(math::vec3(0));
  EXPECT_NE(node->getPosition(), camera->getPosition());

  // Clone node
  auto nodeClone = node->clone();
  nodeClone->setName("copy");
  EXPECT_EQ(nodeClone->getName(), "copy");
  EXPECT_EQ(node->getName(), "CameraController");

  // Test RTTI
  EXPECT_TRUE(isa<CameraController>(node.get()));
}

} // anonymous namespace
