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
#include "sequoia/Core/Logging.h"
#include "sequoia/Game/AssetManager.h"
#include "sequoia/Game/CameraControllerFree.h"
#include "sequoia/Game/Drawable.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/DrawCommandList.h"
#include "sequoia/Render/RenderWindow.h"

#include "sequoia/Render/Texture.h"

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

  std::shared_ptr<SceneNode> cubeOrigin = SceneNode::create("TestCubeOrigin");
  cubeOrigin->addCapability<Drawable>(cubeMesh);
  cubeOrigin->get<Drawable>()->setTexture(
      0, game.createTexture(game.getAssetManager()->loadImage("sequoia/texture/UVTest512x512.dds")));
  sceneGraph_->insert(cubeOrigin);

//  float dx = 3.0f;
//  int N = 10;
//  for(int i = 0; i < N; ++i) {
//    for(int j = 0; j < N; ++j) {
//      auto cube = SceneNode::create(core::format("TestCube_%i_%i", i, j));
//      cube->addCapability<Drawable>(cubeMesh, game.getDefaultProgram());
//      cube->translate(math::vec3((i - N / 2.0f) * dx, 0, (j - N / 2.0f) * dx));
//      cube->setScale(float(i) / N);
//      cube->get<Drawable>()->setTexture(
//          0, game.createTexture(game.getAssetManager()->loadImage("sequoia/texture/Gaben.png")));
//      sceneGraph_->insert(cube);
//    }
//  }

  auto controller = SceneNode::create<CameraControllerFree>("Camera");
  controller->setCamera(activeCamera_);
  sceneGraph_->insert(controller);

  game.getMainWindow()->setCursorMode(render::RenderTarget::CK_Disabled);
}

void Scene::updateDrawCommandList(render::DrawCommandList* list) {
  drawCommandList_.clear();

  // Extract all DrawCommands
  sceneGraph_->apply([this](SceneNode* node) {
    if(Drawable* draw = node->get<Drawable>()) {
      if(draw->isActive()) {
        drawCommandList_.emplace_back(draw->prepareDrawCommand());
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
