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

#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Core/Casting.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Game/AssetManager.h"
#include "sequoia-engine/Game/CameraControllerFree.h"
#include "sequoia-engine/Game/Drawable.h"
#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Game/MeshManager.h"
#include "sequoia-engine/Game/SceneGraph.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/DrawCommandList.h"
#include "sequoia-engine/Render/RenderCommand.h"
#include "sequoia-engine/Render/RenderWindow.h"
#include <functional>

// TODO: Remove me
#include "sequoia-engine/Render/Texture.h"

namespace sequoia {

namespace game {

Scene::Scene() : activeCamera_(nullptr) {
  renderCommand_.first().setDrawCommandList(std::make_shared<render::DrawCommandListDefault>());
  renderCommand_.second().setDrawCommandList(std::make_shared<render::DrawCommandListDefault>());
  drawCommands_.reserve(render::DrawCommandList::DefaultSize);

  sceneGraph_ = std::make_shared<SceneGraph>();
}

void Scene::populateRenderCommand(render::RenderCommand* command) {
  // Reset temporaries
  drawCommands_.clear();
  for(int i = 0; i < Emittable::NumEmitter; ++i)
    emitters_[i] = 0;

  // Traverse SceneGraph and extract DrawCommands and register the Emitters
  sceneGraph_->apply(
      [this](SceneNode* node) {
        if(Drawable* draw = node->get<Drawable>()) {
          if(draw->isActive()) {
            drawCommands_.push_back(draw->prepareDrawCommand());
          }
        }

        if(Emittable* emittable = node->get<Emittable>()) {
          if(emittable->isActive()) {
            int index = std::atomic_fetch_add(&emitters_[emittable->getKind()], 1);
            (void)index;
            // emittable->toUniformVariableMap(, index);
          }
        }
      },
      SceneNode::EP_Parallel);

  // Copy DrawCommands
  command->getDrawCommandList()->insert(drawCommands_);
}

void Scene::setActiveCamera(const std::shared_ptr<render::Camera>& camera) {
  activeCamera_ = camera;
  for(auto listener : getListeners<SceneListener>())
    listener->sceneListenerActiveCameraChanged(this);
}

const std::shared_ptr<render::Camera>& Scene::getActiveCamera() const { return activeCamera_; }

SceneGraph* Scene::getSceneGraph() const { return sceneGraph_.get(); }

render::RenderCommand* Scene::prepareRenderCommand(render::RenderTarget* target) noexcept {
  render::RenderCommand* command = &renderCommand_.get();
  command->reset();

  command->setRenderTarget(target);
  populateRenderCommand(command);

  renderCommand_.nextTimestep(false);
  return command;
}

void Scene::update() {}

void Scene::updateImpl() {
  // Inform the nodes to progress to the next time-step
  SceneNode::UpdateEvent event{0.1f};
  sceneGraph_->update(event);

  // Update the scene
  update();
}

void Scene::makeDummyScene() {
  Game& game = Game::getSingleton();

  // Create the camera
  std::shared_ptr<render::Camera> camera = std::make_shared<render::Camera>(math::vec3(0, 5, 5));
  camera->setZNearClipping(0.1f);
  setActiveCamera(camera);

  std::shared_ptr<SceneNode> cubeOrigin = SceneNode::allocate("TestCubeOrigin");

  //
  // UV coord cube
  //
  MeshParameter cubeMeshParam;
  cubeMeshParam.TexCoordInvertV = true;

  cubeOrigin->addCapability<Drawable>(
      game.getMeshManager()->createCube("TestCubeInvertedV", false, cubeMeshParam));
  cubeOrigin->get<Drawable>()->setTexture(0, game.createTexture(game.getAssetManager()->loadImage(
                                                 "sequoia/texture/UVTest512x512.dds")));
  sceneGraph_->insert(cubeOrigin);

  //
  // Mini cubes
  //
  auto cubeMesh = game.getMeshManager()->createCube("TestCube");
  float dx = 3.0f;
  int N = 10;
  for(int i = 0; i < N; ++i) {
    for(int j = 0; j < N; ++j) {
      auto cube = SceneNode::allocate(core::format("TestCube_{}_{}", i, j));
      cube->addCapability<Drawable>(cubeMesh, game.getDefaultProgram());
      cube->translate(math::vec3((i - N / 2.0f) * dx, 0, (j - N / 2.0f) * dx));
      cube->setScale(0.9 * (2 * float(i) / N));
      cube->get<Drawable>()->setTexture(
          0, game.createTexture(game.getAssetManager()->loadImage("sequoia/texture/Gaben.jpg")));
      sceneGraph_->insert(cube);
    }
  }

  //
  // Cornell box
  //
  std::shared_ptr<SceneNode> cornellBox = SceneNode::allocate("CornellBox");
  cornellBox->addCapability<Drawable>(game.getMeshManager()->load(
      "CornellBox", game.getAssetManager()->load("sequoia/mesh/CornellBox.obj"),
      game.getAssetManager()->load("sequoia/mesh/CornellBox.mtl")));
  cornellBox->translate(math::vec3(13.0f, 0.0f, 13.0f));
  cornellBox->setScale(0.01f);
  sceneGraph_->insert(cornellBox);

  //    //
  //    // Grid
  //    //
  //    std::shared_ptr<SceneNode> gridOrigin = SceneNode::allocate("TestGridOrigin");
  //    gridOrigin->addCapability<Drawable>(game.getMeshManager()->createGrid("TestGrid", 33));
  //    gridOrigin->translate(math::vec3(0.0f, -1.0f, 0.0f));
  //    gridOrigin->setScale(50);
  //    render::TextureParameter texParam;
  //    texParam.Dim1EdgeSampling = texParam.Dim2EdgeSampling =
  //        render::TextureParameter::EK_MirroredRepeat;

  //    gridOrigin->get<Drawable>()->setProgram(game.createProgram(
  //        {{render::Shader::ST_Vertex,
  //        game.getAssetManager()->load("sequoia/shader/MultiTex.vert")},
  //         {render::Shader::ST_Fragment,
  //          game.getAssetManager()->load("sequoia/shader/MultiTex.frag")}}));
  //    gridOrigin->get<Drawable>()->setTexture(
  //        0, game.createTexture(game.getAssetManager()->loadImage("sequoia/texture/SandTex0.png"),
  //                              texParam));
  //    gridOrigin->get<Drawable>()->setTexture(
  //        1,
  //        game.createTexture(game.getAssetManager()->loadImage("sequoia/texture/SandTex1.tiff"),
  //                              texParam));
  //    sceneGraph_->insert(gridOrigin);

  auto controller = SceneNode::allocate<CameraControllerFree>("Camera");
  controller->setCamera(activeCamera_);
  sceneGraph_->insert(controller);

  game.getMainWindow()->setCursorMode(render::RenderWindow::CK_Disabled);
}

} // namespace game

} // namespace sequoia