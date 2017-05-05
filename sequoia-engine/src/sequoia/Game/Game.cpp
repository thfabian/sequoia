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

#include "sequoia/Core/Logging.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Render/RenderSystem.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(game::Game);

namespace game {

Game::Game() : renderSystem_(nullptr) { initialize(); }

Game::~Game() { cleanup(); }

void Game::run() {
  while(renderSystem_->getWindow(mainWindowID_)->isOpen()) {
  }
}

void Game::initialize() {
  LOG(INFO) << "Initializing Game ...";

  // Initialize the RenderSystem
  renderSystem_ = std::make_shared<render::RenderSystem>(render::RenderSystem::RK_OpenGL);

  // Initialize the main-window
  mainWindowID_ = renderSystem_->createWindow(320, 240, "a title");

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
