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
#include "sequoia/Core/Format.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/DrawCommandList.h"
#include "sequoia/Render/RenderWindow.h"

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

  auto cubeMesh = game.getMeshManager()->createCube("TestCube");

  auto cubeOrigin = SceneGraph::create<Drawable>("TestCubeOrigin");
  cubeOrigin->setProgram(game.getDefaultProgram());
  cubeOrigin->setMesh(cubeMesh);
  sceneGraph_->insert(cubeOrigin);

  float dx = 3.0f;
  int N = 10;
  for(int i = 0; i < N; ++i) {
    for(int j = 0; j < N; ++j) {
      auto cube = SceneGraph::create<Drawable>(core::format("TestCube_%i_%i", i, j));
      cube->setProgram(game.getDefaultProgram());
      cube->setMesh(cubeMesh);
      cube->translate(math::vec3((i - N / 2) * dx, 0, (j - N / 2) * dx));
      cube->setScale(float(i) / N);
      sceneGraph_->insert(cube);
    }
  }

  auto controller = SceneGraph::create<CameraControllerFree>("Camera");
  controller->setCamera(activeCamera_);
  sceneGraph_->insert(controller);

  game.getMainWindow()->setCursorMode(render::RenderTarget::CK_Disabled);
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
