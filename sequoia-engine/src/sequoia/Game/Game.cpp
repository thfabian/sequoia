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

namespace {

/// @brief Listen to changes of the camera of the current scene
class CameraChangeListener {};
}

namespace game {

Game::Game(std::string name)
    : renderSystem_(nullptr), assetManager_(nullptr), meshManager_(nullptr), mainWindow_(nullptr),
      quitKey_(nullptr), shouldClose_(false), activeScene_(nullptr), name_(name) {}

Game::~Game() { cleanup(); }

void Game::run() {
  LOG(INFO) << "Starting main-loop ...";

  render::DrawCommandList* drawCommandList = mainWindow_->getDrawCommandList().get();

  // Start main-loop
  while(!mainWindow_->isClosed() && !shouldClose_) {

    // Set the draw commands
    drawCommandList->clear();
    activeScene_->updateDrawCommandList(drawCommandList);

    // Start rendering to the main-window
    renderSystem_->renderOneFrame(mainWindow_);

    // Compute next time-step
    activeScene_->updateImpl();

    // Update screen
    mainWindow_->swapBuffers();

    // Query I/O events
    renderSystem_->pollEvents();
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

    // Create the main-window & initialize the OpenGL context
    RenderWindow::WindowHint hint;
    hint.Title = "Sequoia - " SEQUOIA_VERSION_STRING;
    hint.Monitor = opt.Render.Monitor;
    using WindowModeKind = render::RenderWindow::WindowHint::WindowModeKind;
    hint.WindowMode = core::StringSwitch<WindowModeKind>(opt.Render.WindowMode)
                          .Case("fullscreen", WindowModeKind::WK_Fullscreen)
                          .Case("windowed-fullscreen", WindowModeKind::WK_WindowedFullscreen)
                          .Default(WindowModeKind::WK_Window);
    hint.HideWindow = hideWindow;

    mainWindow_ = renderSystem_->createMainWindow(hint);

//    // Set the viewport of the mainwindow
//    auto viewport = std::make_shared<render::Viewport>(mainWindow_, 0, 0, mainWindow_->getWidth(),
//                                                       mainWindow_->getHeight());
//    mainWindow_->setViewport(viewport);

//    // Initialize the main-window
//    mainWindow_->init();

    // Register the game as a keyboard and mouse listener
    renderSystem_->addKeyboardListener(this);
    renderSystem_->addMouseListener(this);

    // Initialize the object managers
    assetManager_ =
        std::make_unique<AssetManager>(PLATFORM_STR(SEQUOIA_RESSOURCEPATH), PLATFORM_STR("assets"));
    meshManager_ = std::make_unique<MeshManager>();

    // Create default shaders and program
    renderSystem_->loadDefaultShaders(assetManager_->load("sequoia/shader/Default.vert"),
                                      assetManager_->load("sequoia/shader/Default.frag"));

    // -- tmp --
    setScene("TestScene", std::make_shared<Scene>(), true);
    getActiveScene()->makeDummyScene();
    quitKey_ = Keymap::makeDefault(render::Key_Q, render::Mod_Ctrl);

  } catch(core::Exception& e) {
    ErrorHandler::getSingleton().fatal(e.what());
  }

  LOG(INFO) << "Done initializing " << name_;
}

void Game::cleanup() {
  LOG(INFO) << "Terminating " << name_ << " ...";

  // Free all the scenes
  sceneMap_.clear();

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

AssetManager* Game::getAssetManager() const { return assetManager_.get(); }

MeshManager* Game::getMeshManager() const { return meshManager_.get(); }

render::RenderWindow* Game::getMainWindow() const { return mainWindow_; }

render::Texture* Game::createTexture(const std::shared_ptr<Image>& image) {
  return createTexture(image, render::TextureParameter());
}

render::Texture* Game::createTexture(const std::shared_ptr<Image>& image,
                                     const render::TextureParameter& param) {
  return renderSystem_->createTexture(image, param).get();
}

const std::shared_ptr<render::Shader>& Game::getDefaultVertexShader() const {
  return renderSystem_->getDefaultVertexShader();
}

const std::shared_ptr<render::Shader>& Game::getDefaultFragmentShader() const {
  return renderSystem_->getDefaultFragmentShader();
}

const std::shared_ptr<render::Program>& Game::getDefaultProgram() const {
  return renderSystem_->getDefaultProgram();
}

Scene* Game::getActiveScene() const { return activeScene_; }

void Game::setScene(const std::string& name, const std::shared_ptr<Scene>& scene, bool makeActive) {
  LOG(INFO) << "Inserting new scene \"" << name << "\" into game ";
  sceneMap_.emplace(name, scene);
  if(makeActive)
    makeSceneActive(name);
}

Scene* Game::getScene(const std::string& name) const {
  auto it = sceneMap_.find(name);
  SEQUOIA_ASSERT_MSG(it != sceneMap_.end(), "requested Scene does not exist");
  return it->second.get();
}

void Game::removeScene(const std::string& name) {
  LOG(INFO) << "Removing scene \"" << name << "\" from game ";
  sceneMap_.erase(name);
}

void Game::makeSceneActive(const std::string& name) {
  if(activeScene_)
    activeScene_->removeListener<SceneListener>(this);

  activeScene_ = getScene(name);
  activeScene_->addListener<SceneListener>(this);
}

void Game::sceneListenerActiveCameraChanged(Scene* scene) {
  // Inform the view port of the main-window about the changed camera
  mainWindow_->getViewport()->setCamera(scene->getActiveCamera().get());
}

} // namespace game

} // namespace sequoia
