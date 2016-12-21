//===-- sequoia/Game/Game.h ---------------------------------------------------------*- C++ -*-===//
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

#ifndef SEQUOIA_GAME_GAME_H
#define SEQUOIA_GAME_GAME_H

#include "sequoia/Core/Core.h"
#include <OGRE/OgrePrerequisites.h>
#include <OGRE/OgreSingleton.h>
#include <memory>

namespace sequoia {

namespace game {

class SEQUOIA_EXPORT Game : public Ogre::Singleton<Game> {
public:
  /// @brief Initialize game
  Game();

  /// @brief Clean up
  ~Game();

  /// @brief
  void run();

private:
  Ogre::Root* root_;
  Ogre::RenderWindow* renderWindow_;
  Ogre::SceneManager* sceneManager_;
  Ogre::Camera* camera_;
};

} // namespace game

} // namespace sequoia

#endif
