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
#include "sequoia/Game/Game.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/DrawCommandList.h"

namespace sequoia {

namespace game {

Scene::Scene() : camera_(nullptr) {
  drawCommandList_.reserve(render::DrawCommandList::DefaultSize);

  Game& game = Game::getSingleton();
  sceneGraph_ = std::make_shared<SceneGraph>();

  //
  // Test Scene
  //

  // Create the camera
  cameraList_.emplace_back(std::make_shared<render::Camera>());
  camera_ = cameraList_.back().get();
  camera_->setEye(math::vec3(4, 3, -3));
  camera_->setCenter(math::vec3(0, 0, 0));

  auto node = SceneGraph::create<SceneNodeDrawable>("TestNode");
  node->setProgram(game.getDefaultProgram());
  
  auto mesh = game.getMeshManager()->createCube("TestCube");
  node->setMesh(mesh);

  sceneGraph_->insert(node);
}

void Scene::updateDrawCommandList(render::DrawCommandList* list) {
  drawCommandList_.clear();

  // Extract all DrawCommands
  sceneGraph_->apply([this](SceneNode* node) {
    if(SceneNodeDrawable* drawNode = dyn_cast<SceneNodeDrawable>(node)) {
      drawCommandList_.emplace_back(drawNode->getDrawCommand().get());
    }
  });

  // Copy DrawCommands
  list->insert(drawCommandList_);
}

void Scene::addCamera(const std::shared_ptr<render::Camera>& camera) {
  cameraList_.push_back(camera);
}

render::Camera* Scene::getCamera() const { return camera_; }

SceneGraph* Scene::getSceneGraph() const { return sceneGraph_.get(); }

} // namespace game

} // namespace sequoia
