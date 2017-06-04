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

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Core/StringSwitch.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/DrawCommandList.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/RenderSystem.h"
#include "sequoia/Render/RenderWindow.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(game::Game);

namespace game {

Game::Game() : renderSystem_(nullptr), mainWindow_(nullptr), scene_(nullptr) {}

Game::~Game() { cleanup(); }

void Game::run() {
  LOG(INFO) << "Starting main-loop ...";

  // Start main-loop
  while(!mainWindow_->isClosed()) {

    // Set the draw commands
    render::DrawCommandList* list = mainWindow_->getDrawCommandList().get();
    list->clear();
    scene_->updateDrawCommandList(list);

    // Start rendering all tender targets
    renderSystem_->renderOneFrame();

    // Update screen
    renderSystem_->swapBuffers();

    // Query I/O events
    renderSystem_->pollEvents();
  }

  LOG(INFO) << "Done with main-loop";
}

void Game::init(bool hideWindow) {
  LOG(INFO) << "Initializing Game ...";
  using namespace render;

  Options& opt = Options::getSingleton();

  try {
    // Initialize the RenderSystem
    renderSystem_ = RenderSystem::create(RK_OpenGL);

    if(Options::getSingleton().Core.Debug)
      renderSystem_->setDebugMode(true);

    // Create the main-window
    RenderWindow::WindowHint hint;
    hint.Title = "Sequoia - " SEQUOIA_VERSION_STRING;
    hint.Monitor = opt.Render.Monitor;
    using WindowModeKind = render::RenderWindow::WindowHint::WindowModeKind;
    hint.WindowMode = core::StringSwitch<WindowModeKind>(opt.Render.WindowMode)
                          .Case("fullscreen", WindowModeKind::WK_Fullscreen)
                          .Case("windowed-fullscreen", WindowModeKind::WK_WindowedFullscreen)
                          .Default(WindowModeKind::WK_Window);
    hint.MSAA = opt.Render.MSAA;
    hint.HideWindow = hideWindow;

    hint.GLMajorVersion = opt.Render.GLMajorVersion;
    hint.GLMinorVersion = opt.Render.GLMinorVersion;

    mainWindow_ = renderSystem_->createWindow(hint);

    // Set the viewport of the mainwindow
    auto viewport = std::make_shared<render::Viewport>(mainWindow_, 0, 0, mainWindow_->getWidth(),
                                                       mainWindow_->getHeight());
    mainWindow_->setViewport(viewport);

    // Initialize the main-window
    mainWindow_->init();

    // Create default shaders and program
//    defaultVertexShader_ = renderSystem_->loadShader(mainWindow_, Shader::ST_Vertex,
//                                                     "assets/sequoia/shader/Default.frag");
//    defaultFragmentShader_ =
//        renderSystem_->loadShader(mainWindow_, Shader::ST_Fragment, "assets/sequoia/Default.frag");
//    defaultProgram_ =
//        renderSystem_->createProgram(mainWindow_, {defaultVertexShader_, defaultFragmentShader_});

    // Register the game as a keyboard and mouse listener
    renderSystem_->addKeyboardListener(mainWindow_, this);
    renderSystem_->addMouseListener(mainWindow_, this);

    // Initialize the mesh manager
    meshManager_ = std::make_unique<MeshManager>();

    // Initialize the startup scene
    sceneList_.emplace_back(std::make_shared<Scene>());
    scene_ = sceneList_.back().get();

    // TODO: This is not optimal. The Viewport should automatically be informed if the camera of
    // the scene changes
    mainWindow_->getViewport()->setCamera(scene_->getCamera());

  } catch(render::RenderSystemException& e) {
    ErrorHandler::getSingleton().fatal(e.what());
  }

  LOG(INFO) << "Done initializing Game";
}

void Game::cleanup() {
  LOG(INFO) << "Terminating Game ...";

  // Free RenderSystem
  renderSystem_.reset();

  LOG(INFO) << "Done terminating Game";
}

void Game::keyboardEvent(const render::KeyboardEvent& event) {}

void Game::mouseEvent(const render::MouseEvent& event) {}

MeshManager* Game::getMeshManager() const { return meshManager_.get(); }

render::RenderWindow* Game::getMainWindow() const { return mainWindow_; }

render::RenderTarget* Game::getMainRenderTarget() const {
  return static_cast<render::RenderTarget*>(mainWindow_);
}

Scene* Game::getScene() const { return scene_; }

} // namespace game

} // namespace sequoia
