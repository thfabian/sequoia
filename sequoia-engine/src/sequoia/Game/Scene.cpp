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

#include "sequoia/Game/Scene.h"
#include "sequoia/Core/Casting.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/DrawCommandList.h"

#include "sequoia/Game/CameraControllerFree.h"
#include "sequoia/Game/Drawable.h"

namespace sequoia {

namespace game {

Scene::Scene() : activeCamera_(nullptr) {
  drawCommandList_.reserve(render::DrawCommandList::DefaultSize);

  Game& game = Game::getSingleton();
  sceneGraph_ = std::make_shared<SceneGraph>();

  //
  // Test Scene
  //

  // Create the camera
  activeCamera_ = std::make_shared<render::Camera>();
  activeCamera_->setEye(math::vec3(0, 0, -20));
  activeCamera_->setCenter(math::vec3(0, 0, 0));

  auto cube = SceneGraph::create<Drawable>("TestNode");
  cube->setProgram(game.getDefaultProgram());

  auto mesh = game.getMeshManager()->createCube("TestCube");
  cube->setMesh(mesh);
  sceneGraph_->insert(cube);

  auto controller = SceneGraph::create<CameraControllerFree>("Camera");
  controller->setCamera(activeCamera_);
  sceneGraph_->insert(controller);
}

void Scene::updateDrawCommandList(render::DrawCommandList* list) {
  drawCommandList_.clear();

  // Extract all DrawCommands
  sceneGraph_->apply([this](SceneNode* node) {
    if(Drawable* drawNode = dyn_cast<Drawable>(node)) {
      if(drawNode->isActive()) {
        drawCommandList_.emplace_back(drawNode->prepareDrawCommand());
      }
    }
  });

  // Copy DrawCommands
  list->insert(drawCommandList_);
}

void Scene::setActiveCamera(const std::shared_ptr<render::Camera>& camera) {
  activeCamera_ = camera;
}

const std::shared_ptr<render::Camera>& Scene::getActiveCamera() const { return activeCamera_; }

SceneGraph* Scene::getSceneGraph() const { return sceneGraph_.get(); }

void Scene::update() {
  // Inform the nodes to progress to the next time-step
  SceneNode::UpdateEvent event{0.1f};
  sceneGraph_->update(event);
}

} // namespace game

} // namespace sequoia
