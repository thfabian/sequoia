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
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Game/Drawable.h"
#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Game/SceneGraph.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/RenderCommand.h"

namespace sequoia {

namespace game {

Scene::Scene(const std::string& name)
    : name_(name), sceneGraph_(std::make_shared<SceneGraph>()), activeCamera_(nullptr) {}

void Scene::setActiveCamera(const std::shared_ptr<render::Camera>& camera) {
  activeCamera_ = camera;
  for(auto listener : getListeners<SceneListener>())
    listener->sceneListenerActiveCameraChanged(this);
}

const std::shared_ptr<render::Camera>& Scene::getActiveCamera() const { return activeCamera_; }

SceneGraph* Scene::getSceneGraph() const { return sceneGraph_.get(); }

void Scene::prepareRenderTechniques(std::vector<render::RenderTechnique*>& techiques) {}

void Scene::prepareRenderTarget(render::RenderTarget*& target) {}

void Scene::prepareDrawCommands(std::vector<render::DrawCommand>& drawCommands) {
  sceneGraph_->apply(
      [&drawCommands](SceneNode* node) {
        if(Drawable* drawable = node->get<Drawable>()) {
          if(drawable->isActive()) {
            drawable->prepareDrawCommands(drawCommands);
          }
        }
      },
      SceneNode::EP_Parallel);
}

void Scene::prepareRenderCommand(render::RenderCommand& cmd) {
  prepareRenderTarget(cmd.Target);
  prepareRenderTechniques(cmd.Techniques);
  prepareDrawCommands(cmd.DrawCommands);
}

void Scene::update() {}

void Scene::updateImpl(float timeStep) {
  // Inform the nodes to progress to the next time-step
  sceneGraph_->update(SceneNode::UpdateEvent{timeStep});

  // Update the scene
  update();
}

} // namespace game

} // namespace sequoia
