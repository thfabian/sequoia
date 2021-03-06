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

#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Timer.h"
#include "sequoia-engine/Game/AssetManager.h"
#include "sequoia-engine/Game/CameraControllerFree.h"
#include "sequoia-engine/Game/Drawable.h"
#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Game/SceneGraph.h"
#include "sequoia-engine/Game/ShapeManager.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/RenderWindow.h"
#include "sequoia-engine/Render/Texture.h"
#include "sequoia-examples/Sandbox/SandboxScene.h"

using namespace sequoia;

namespace sequoia_examples {

namespace sandbox {

SandboxScene::SandboxScene(const std::string& name) : Scene(name) {
  game::Game& game = game::Game::getSingleton();

  Log::info("Initializing scene {} ...", getName());
  Timer timer;

  // Create RenderTechniques
  rtDefault_ = std::make_unique<render::RTDefault>();

  // Create the camera
  std::shared_ptr<render::Camera> camera = std::make_shared<render::Camera>(math::vec3(0, 5, 5));
  camera->setZNearClipping(0.1f);
  setActiveCamera(camera);

  // UV coord cube
  std::shared_ptr<game::SceneNode> cubeOrigin = game::SceneNode::allocate("TestCubeOrigin");

  game::MeshParameter cubeOriginParam;
  cubeOriginParam.TexCoordInvertV = true;

  auto cubeOriginShape =
      game.getShapeManager()->createCube("TestCubeInvertedV", false, cubeOriginParam);

  // TODO: improve the API
  cubeOriginShape->getMaterials().front()->setTexture(
      0, game.createTexture(game.getAssetManager()->loadImage("texture/UVTest512x512.dds")));

  cubeOrigin->addCapability<game::Drawable>(cubeOriginShape);
  getSceneGraph()->insert(cubeOrigin);

  // Mini cubes
  auto cubeShape = game.getShapeManager()->createCube("TestCube");
  float dx = 3.0f;
  int N = 10;
  for(int i = 0; i < N; ++i) {
    for(int j = 0; j < N; ++j) {
      auto cube = game::SceneNode::allocate(core::format("TestCube_{}_{}", i, j));

      cube->addCapability<game::Drawable>(cubeShape);
      cube->translate(math::vec3((i - N / 2.0f) * dx, 0, (j - N / 2.0f) * dx));
      cube->setScale(0.9 * (2 * float(i) / N));

      cubeShape->getMaterials().front()->setTexture(
          0, game.createTexture(game.getAssetManager()->loadImage("texture/Gaben.jpg")));

      getSceneGraph()->insert(cube);
    }
  }

  //
  // Cornell box
  //
  //  std::shared_ptr<SceneNode> cornellBox = SceneNode::allocate("CornellBox");
  //  cornellBox->addCapability<Drawable>(game.getMeshManager()->load(
  //      "CornellBox", game.getAssetManager()->load("sequoia/mesh/CornellBox.obj"),
  //      game.getAssetManager()->load("sequoia/mesh/CornellBox.mtl")));
  //  cornellBox->translate(math::vec3(13.0f, 0.0f, 13.0f));
  //  cornellBox->setScale(0.01f);
  //  sceneGraph_->insert(cornellBox);

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

  auto controller = game::SceneNode::allocate<game::CameraControllerFree>("Camera");
  controller->setCamera(getActiveCamera());
  getSceneGraph()->insert(controller);

  game.getMainWindow()->setCursorMode(render::RenderWindow::CK_Disabled);

  Log::info("Done initializing scene {} (took {})", getName(), core::timeString(timer.stop()));
}

void SandboxScene::update() {}

void SandboxScene::prepareRenderTechniques(std::vector<render::RenderTechnique*>& techiques) {
  techiques.emplace_back(rtDefault_.get());
}

} // namespace sandbox

} // namespace sequoia_examples
