//===-- sequoia/Game/Game.cpp -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Main class holding all Ogre related objects.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Game/Game.h"
#include <OGRE/Ogre.h>

template <>
sequoia::game::Game* Ogre::Singleton<sequoia::game::Game>::msSingleton = nullptr;

namespace sequoia {

namespace game {

Game::Game() {
  root_ = new Ogre::Root();  
}

Game::~Game() {
  delete root_;
}

void Game::run() {
  
}

} // namespace game

} // namespace sequoia
