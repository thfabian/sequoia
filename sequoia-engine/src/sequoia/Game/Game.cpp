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

#include "sequoia/Game/Game.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(game::Game);

namespace game {

Game::Game() {
  initialize();
}

Game::~Game() {
  cleanup();
}

void Game::run() {

}

void Game::initialize() {
  
}

void Game::cleanup() {
  
}

} // namespace game

} // namespace sequoia
