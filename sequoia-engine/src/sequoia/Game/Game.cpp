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

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Core/StringSwitch.h"
#include "sequoia/Game/AssetManager.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/Keymap.h"
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

Game::Game(std::string name)
    : renderSystem_(nullptr), meshManager_(nullptr), assetManager_(nullptr), mainWindow_(nullptr),
      quitKey_(nullptr), shouldClose_(false), scene_(nullptr), name_(name) {}

Game::~Game() { cleanup(); }

void Game::run() {
  LOG(INFO) << "Starting main-loop ...";

  render::DrawCommandList* drawCommandList = mainWindow_->getDrawCommandList().get();

  // Start main-loop
  while(!mainWindow_->isClosed() && !shouldClose_) {

    // Query I/O events
    renderSystem_->pollEvents();

    // Set the draw commands
    drawCommandList->clear();
    scene_->updateDrawCommandList(drawCommandList);

    // Compute next time-step
    scene_->update();

    // Start rendering all tender targets
    renderSystem_->renderOneFrame();

    // Update screen
    renderSystem_->swapBuffers();
  }

  LOG(INFO) << "Done with main-loop";
}

void Game::setQuitKey(const std::shared_ptr<Keymap>& key) { quitKey_ = key; }

void Game::init(bool hideWindow) {
  LOG(INFO) << "Initializing " << name_ << " ...";
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
    hint.HideWindow = hideWindow;

    mainWindow_ = renderSystem_->createWindow(hint);

    // Set the viewport of the mainwindow
    auto viewport = std::make_shared<render::Viewport>(mainWindow_, 0, 0, mainWindow_->getWidth(),
                                                       mainWindow_->getHeight());
    mainWindow_->setViewport(viewport);

    // Initialize the main-window
    mainWindow_->init();
    quitKey_ = Keymap::makeDefault(render::Key_Q, render::Mod_Ctrl);

    // Register the game as a keyboard and mouse listener
    renderSystem_->addKeyboardListener(mainWindow_, this);
    renderSystem_->addMouseListener(mainWindow_, this);

    // Initialize the mesh manager
    meshManager_ = std::make_unique<MeshManager>(mainWindow_);
    assetManager_ =
        std::make_unique<AssetManager>(PLATFORM_STR(SEQUOIA_RESSOURCEPATH), PLATFORM_STR("assets"));

    // Create default shaders and program
    renderSystem_->loadDefaultShaders(mainWindow_,
                                      assetManager_->load("sequoia/shader/Default.vert"),
                                      assetManager_->load("sequoia/shader/Default.frag"));

    // Initialize the startup scene
    sceneList_.emplace_back(std::make_shared<Scene>());
    scene_ = sceneList_.back().get();

    // TODO: This is not optimal. The Viewport should automatically be informed if the camera of
    // the scene changes
    mainWindow_->getViewport()->setCamera(scene_->getActiveCamera().get());

  } catch(render::RenderSystemException& e) {
    ErrorHandler::getSingleton().fatal(e.what());
  }

  LOG(INFO) << "Done initializing " << name_;
}

void Game::cleanup() {
  LOG(INFO) << "Terminating " << name_ << " ...";

  // Free all the scenes
  sceneList_.clear();

  // Free managers
  meshManager_.reset();
  assetManager_.reset();

  // Free all RenderSystem objects
  renderSystem_.reset();

  LOG(INFO) << "Done terminating " << name_;
}

void Game::keyboardEvent(const render::KeyboardEvent& event) {
  if(quitKey_ && quitKey_->handle(event))
    shouldClose_ = true;

  for(KeyboardListener* listener : getListeners<KeyboardListener>())
    listener->keyboardEvent(event);
}

void Game::mouseButtonEvent(const render::MouseButtonEvent& event) {
  for(MouseListener* listener : getListeners<MouseListener>())
    listener->mouseButtonEvent(event);
}

void Game::mousePositionEvent(const render::MousePositionEvent& event) {
  for(MouseListener* listener : getListeners<MouseListener>())
    listener->mousePositionEvent(event);
}

MeshManager* Game::getMeshManager() const { return meshManager_.get(); }

AssetManager* Game::getAssetManager() const { return assetManager_.get(); }

render::RenderWindow* Game::getMainWindow() const { return mainWindow_; }

render::RenderTarget* Game::getMainRenderTarget() const {
  return static_cast<render::RenderTarget*>(mainWindow_);
}

const std::shared_ptr<render::Shader>& Game::getDefaultVertexShader() const {
  return renderSystem_->getDefaultVertexShader(mainWindow_);
}

const std::shared_ptr<render::Shader>& Game::getDefaultFragmentShader() const {
  return renderSystem_->getDefaultFragmentShader(mainWindow_);
}

const std::shared_ptr<render::Program>& Game::getDefaultProgram() const {
  return renderSystem_->getDefaultProgram(mainWindow_);
}

Scene* Game::getScene() const { return scene_; }

} // namespace game

} // namespace sequoia
