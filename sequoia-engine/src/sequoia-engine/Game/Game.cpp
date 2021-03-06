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

#include "sequoia-engine/Core/ErrorHandler.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/Platform.h"
#include "sequoia-engine/Core/StringSwitch.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Timer.h"
#include "sequoia-engine/Core/Version.h"
#include "sequoia-engine/Game/AssetManager.h"
#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Game/Keymap.h"
#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Game/ShapeManager.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/RenderSystem.h"
#include "sequoia-engine/Render/RenderWindow.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(game::Game);

namespace game {

Game::Game()
    : renderSystem_(nullptr), assetManager_(nullptr), shapeManager_(nullptr), mainWindow_(nullptr),
      quitKey_(nullptr), shouldClose_(false), activeScene_(nullptr), name_("<unknown>"),
      options_(nullptr) {}

Game::~Game() { cleanup(); }

std::shared_ptr<Options> Game::makeOptions() {
  auto options = std::make_shared<Options>();
  Game::setDefaultOptions(options);
  return options;
}

void Game::run() {
  SEQUOIA_ASSERT_MSG(renderSystem_, "Game not initialized, did you call init()?");

  Log::info("Starting main-loop ...");

  render::RenderCommand cmd(getMainRenderTarget());
  Timer timer;

  // Start main-loop
  while(!mainWindow_->isClosed() && !shouldClose_) {

    // Advance the scene to the next time-step
    activeScene_->updateImpl(0.1f); /* TODO: use actual timestep */

    // Render the scene
    cmd.reset(getMainRenderTarget());
    activeScene_->prepareRenderCommand(cmd);
    renderSystem_->renderOneFrame(cmd);

    // Update screen
    mainWindow_->swapBuffers();

    // Query I/O events
    renderSystem_->pollEvents();
  }

  Log::info("Done with main-loop (took {})", core::timeString(timer.stop()));
}

void Game::setQuitKey(const std::shared_ptr<Keymap>& key) { quitKey_ = key; }

void Game::init(const std::shared_ptr<Options>& gameOptions) {
  options_ = gameOptions;
  Game::setDefaultOptions(options_);

  name_ = options_->getString("Game.Name");

  Log::info("Initializing {} ...", name_);
  Timer timer;

  using namespace render;

  try {
    // Initialize the RenderSystem
    renderSystem_ = RenderSystem::create(
        options_->getEnum<render::RenderSystemKind>("Game.RenderSystem"), options_);

    // Create the main-window & initialize the OpenGL context
    RenderWindow::WindowHint hint;
    hint.Title = name_;
    hint.Monitor = options_->getInt("Render.Monitor");
    using WindowModeKind = render::RenderWindow::WindowHint::WindowModeKind;
    hint.WindowMode = core::StringSwitch<WindowModeKind>(options_->getString("Render.WindowMode"))
                          .Case("fullscreen", WindowModeKind::WK_Fullscreen)
                          .Case("windowed-fullscreen", WindowModeKind::WK_WindowedFullscreen)
                          .Default(WindowModeKind::WK_Window);
    hint.HideWindow = options_->getBool("Game.HideWindow");

    mainWindow_ = renderSystem_->createMainWindow(hint);

    // Register the game as a keyboard and mouse listener
    renderSystem_->addKeyboardListener(this);
    renderSystem_->addMouseListener(this);

    // Initialize the managers
    assetManager_ = std::make_unique<AssetManager>(options_->getString("Game.RessourcePath"));
    shapeManager_ = std::make_unique<ShapeManager>();

  } catch(core::Exception& e) {
    ErrorHandler::fatal(e.what());
  }

  Log::info("Done initializing {} (took {})", name_, core::timeString(timer.stop()));
}

void Game::cleanup() {
  Log::info("Terminating {} ...", name_);

  // Free all the scenes
  sceneMap_.clear();

  // Free managers
  shapeManager_.reset();
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

ShapeManager* Game::getShapeManager() const { return shapeManager_.get(); }

render::RenderWindow* Game::getMainWindow() const { return mainWindow_; }

render::RenderTarget* Game::getMainRenderTarget() const { return getMainWindow(); }

std::shared_ptr<render::Texture> Game::createTexture(const std::shared_ptr<Image>& image,
                                                     const render::TextureParameter& param) {
  return renderSystem_->createTexture(image, param);
}

std::shared_ptr<render::Shader> Game::createShader(render::Shader::ShaderType type,
                                                   const std::string& filename,
                                                   const std::string& source) {
  return renderSystem_->createShader(type, filename, source);
}

std::shared_ptr<render::Program>
Game::createProgram(const std::set<std::shared_ptr<render::Shader>>& shaders) {
  return renderSystem_->createProgram(shaders);
}

Scene* Game::getActiveScene() const { return activeScene_; }

void Game::setScene(const std::shared_ptr<Scene>& scene, bool makeActive) {
  Log::info("Inserting new scene \"{}\" into game ", scene->getName());
  sceneMap_.emplace(scene->getName(), scene);
  if(makeActive)
    makeSceneActive(scene->getName());
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

  // Make sure the Viewport of the main-window has the camera of the scene
  sceneListenerActiveCameraChanged(activeScene_);
}

void Game::sceneListenerActiveCameraChanged(Scene* scene) {
  // Inform the view port of the main-window about the changed camera
  mainWindow_->getViewport()->setCamera(scene->getActiveCamera().get());
}

void Game::setDefaultOptions(const std::shared_ptr<Options>& options) {
  // Core
  core::setDefaultOptions(options);

  // Game
  options->setDefaultBool("Game.HideWindow", false);
  options->setDefaultString("Game.Name", "Game");
  options->setDefaultInt("Game.RenderSystem", render::RK_OpenGL);

  // Render
  render::RenderSystem::setDefaultOptions(options);
}

} // namespace game

} // namespace sequoia
