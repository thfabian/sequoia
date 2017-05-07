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
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/RenderSystem.h"
#include <iostream>

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(game::Game);

namespace game {

Game::Game() : renderSystem_(nullptr) { initialize(); }

Game::~Game() { cleanup(); }

void Game::run() {
  render::RenderWindow* renderWindow = renderSystem_->getWindow(mainWindowID_);

  LOG(INFO) << "Starting main-loop ...";

  // Start main-loop
  while(!renderWindow->isClosed()) {
    
    // Start rendering the frame
    renderWindow->renderOneFrame();

    // Update screen
    renderWindow->swapBuffers();

    // Query I/O events
    renderSystem_->pollEvents();
  }

  LOG(INFO) << "Done with main-loop";
}

void Game::initialize() {
  LOG(INFO) << "Initializing Game ...";

  try {
    // Initialize the RenderSystem
    renderSystem_ = std::make_shared<render::RenderSystem>(render::RenderSystem::RK_OpenGL);

    // Initialize the main-window
    mainWindowID_ = renderSystem_->createWindow("Sequoia - " SEQUOIA_VERSION_STRING);

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
