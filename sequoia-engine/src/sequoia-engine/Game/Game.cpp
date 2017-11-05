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

#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Core/ErrorHandler.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/Platform.h"
#include "sequoia-engine/Core/StringSwitch.h"
#include "sequoia-engine/Core/Version.h"
#include "sequoia-engine/Game/AssetManager.h"
#include "sequoia-engine/Game/Keymap.h"
#include "sequoia-engine/Game/MeshManager.h"
#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/DrawCommandList.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/RenderSystem.h"
#include "sequoia-engine/Render/RenderWindow.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(game::Game);

namespace game {

Game::Game()
    : renderSystem_(nullptr), assetManager_(nullptr), meshManager_(nullptr), mainWindow_(nullptr),
      quitKey_(nullptr), shouldClose_(false), activeScene_(nullptr), name_("<unknown>"),
      options_(nullptr) {}

Game::~Game() { cleanup(); }

void Game::run() {
  SEQUOIA_ASSERT_MSG(renderSystem_, "Game not initialized, did you call init()?");

  Log::info("Starting main-loop ...");

  // Start main-loop
  while(!mainWindow_->isClosed() && !shouldClose_) {

    // Create the render commands
    render::RenderCommand* renderCommand = activeScene_->prepareRenderCommand(mainWindow_);

    // Start rendering to the main-window
    renderSystem_->renderOneFrame(renderCommand);

    // Advance to the next time-step
    activeScene_->updateImpl();

    // Update screen
    mainWindow_->swapBuffers();

    // Query I/O events
    renderSystem_->pollEvents();
  }

  Log::info("Done with main-loop");
}

void Game::setQuitKey(const std::shared_ptr<Keymap>& key) { quitKey_ = key; }

void Game::init(const GameOptions& gameOptions) {
  name_ = gameOptions.Name;
  options_ = gameOptions.Options;

  Options& opt = *options_;

  Log::info("Initializing {} ...", name_);
  using namespace render;

  try {
    // Initialize the RenderSystem
    renderSystem_ = RenderSystem::create(gameOptions.RenderSystemKind, options_);

    // Create the main-window & initialize the OpenGL context
    RenderWindow::WindowHint hint;
    hint.Title = std::string("Sequoia - ") + core::getSequoiaEngineFullVersionString();
    hint.Monitor = opt.get<int>("Render.Monitor");
    using WindowModeKind = render::RenderWindow::WindowHint::WindowModeKind;
    hint.WindowMode = core::StringSwitch<WindowModeKind>(opt.getString("Render.WindowMode"))
                          .Case("fullscreen", WindowModeKind::WK_Fullscreen)
                          .Case("windowed-fullscreen", WindowModeKind::WK_WindowedFullscreen)
                          .Default(WindowModeKind::WK_Window);
    hint.HideWindow = gameOptions.HideWindow;

    mainWindow_ = renderSystem_->createMainWindow(hint);

    // Register the game as a keyboard and mouse listener
    renderSystem_->addKeyboardListener(this);
    renderSystem_->addMouseListener(this);

    // Initialize the object managers
    assetManager_ = std::make_unique<AssetManager>(PLATFORM_STR(SEQUOIA_ENGINE_RESSOURCEPATH),
                                                   PLATFORM_STR("assets"));
    meshManager_ = std::make_unique<MeshManager>();

    // Create default shaders and program
    renderSystem_->loadDefaultShaders(assetManager_->load("sequoia/shader/Default.vert"),
                                      assetManager_->load("sequoia/shader/Default.frag"));

    // -- tmp --
    setScene("TestScene", std::make_shared<Scene>(), true);
    getActiveScene()->makeDummyScene();

    quitKey_ = Keymap::makeDefault(render::Key_Q, render::Mod_Ctrl);

  } catch(core::Exception& e) {
    ErrorHandler::fatal(e.what());
  }

  Log::info("Done initializing {}", name_);
}

void Game::cleanup() {
  Log::info("Terminating {} ...", name_);

  // Free all the scenes
  sceneMap_.clear();

  // Free managers
  meshManager_.reset();
  assetManager_.reset();

  // Free all RenderSystem objects
  renderSystem_.reset();

  Log::info("Done terminating {}", name_);
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

std::shared_ptr<render::Shader> Game::createShader(render::Shader::ShaderType type,
                                                   const std::shared_ptr<File>& source) {
  return renderSystem_->createShader(type, source);
}

std::shared_ptr<render::Program>
Game::createProgram(const std::set<std::shared_ptr<render::Shader>>& shaders) {
  return renderSystem_->createProgram(shaders);
}

std::shared_ptr<render::Program> Game::createProgram(
    std::initializer_list<std::pair<render::Shader::ShaderType, std::shared_ptr<File>>> shaders) {
  std::set<std::shared_ptr<render::Shader>> shaderSet;
  for(const auto& shaderPair : shaders)
    shaderSet.insert(createShader(shaderPair.first, shaderPair.second));
  return createProgram(shaderSet);
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
  Log::info("Inserting new scene \"{}\" into game ", name);
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
  Log::info("Removing scene \"{}\" from game ", name);
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
