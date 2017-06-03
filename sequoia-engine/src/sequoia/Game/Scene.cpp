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

#include "sequoia/Game/Game.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Render/Camera.h"

namespace sequoia {

namespace game {

Scene::Scene() : camera_(nullptr) {
  Game& game = Game::getSingleton();
  sceneGraph_ = std::make_shared<SceneGraph>();

  // Create the camera
  cameraList_.emplace_back(std::make_shared<render::Camera>(math::vec3(0, 1, 0)));
  camera_ = cameraList_.back().get();
  camera_->setEye(math::vec3(4, 3, 3));
  camera_->setCenter(math::vec3(0, 0, 0));

  auto node = SceneGraph::create<SceneNode>("TestNode");
  node->setMesh(game.getMeshManager()->createCube(game.getMainRenderTarget(), "TestCube"));
  sceneGraph_->insert(node);
}

void Scene::updateDrawCommandList(render::DrawCommandList* list) {}

void Scene::addCamera(const std::shared_ptr<render::Camera>& camera) {
  cameraList_.push_back(camera);
}

render::Camera* Scene::getCamera() const { return camera_; }

SceneGraph* Scene::getSceneGraph() const { return sceneGraph_.get(); }

} // namespace game

} // namespace sequoia
