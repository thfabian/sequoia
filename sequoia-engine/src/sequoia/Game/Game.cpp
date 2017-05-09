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
#include "sequoia/Game/Game.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/RenderWindow.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/RenderSystem.h"
#include <iostream>

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(game::Game);

namespace game {

Game::Game() : renderSystem_(nullptr), mainWindow_(nullptr), mainCamera_(nullptr) { init(); }

Game::~Game() { cleanup(); }

void Game::run() {
  LOG(INFO) << "Starting main-loop ...";

  // Start main-loop
  while(!mainWindow_->isClosed()) {

    // Start rendering all tender targets
    renderSystem_->renderOneFrame();

    // Update screen
    renderSystem_->swapBuffers();

    // Query I/O events
    renderSystem_->pollEvents();
  }

  LOG(INFO) << "Done with main-loop";
}

void Game::init() {
  LOG(INFO) << "Initializing Game ...";

  try {
    // Initialize the RenderSystem
    renderSystem_ = render::RenderSystem::create(render::RenderSystem::RK_OpenGL);

    // Create the main-window
    mainWindow_ = renderSystem_->createWindow("Sequoia - " SEQUOIA_VERSION_STRING);

    // Create the camera
    mainCamera_ = std::make_unique<render::Camera>(math::vec3(0, 1, 0));
    mainCamera_->setEye(math::vec3(4, 3, 3));
    mainCamera_->setCenter(math::vec3(0, 0, 0));
    
    // Set the viewport of the mainwindow
    auto viewport = std::make_shared<render::Viewport>(
        mainCamera_.get(), mainWindow_, 0, 0, mainWindow_->getWidth(), mainWindow_->getHeight());
    mainWindow_->setViewport(viewport);
    
    // Initialize the main-window
    mainWindow_->init();

  } catch(render::RenderSystemInitException& e) {
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

} // namespace game

} // namespace sequoia
