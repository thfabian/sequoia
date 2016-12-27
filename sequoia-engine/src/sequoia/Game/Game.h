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

#include "sequoia/Game/Export.h"
#include "sequoia/Game/RenderSubsystem.h"
#include "sequoia/Game/RenderWindow.h"
#include <OGRE/OgreSingleton.h>
#include <memory>

namespace sequoia {

namespace game {

/// @brief Main class holding all Ogre related objects and running the main-loop
class SEQUOIA_GAME_EXPORT Game : public Ogre::Singleton<Game> {
public:
  /// @brief Initialize game
  Game();

  /// @brief Clean up
  ~Game();

  /// @brief Run main-loop
  void run();

private:
  std::shared_ptr<Ogre::Root> root_;
  std::shared_ptr<RenderSubsystem> renderSystem_;
  std::shared_ptr<RenderWindow> renderWindow_;

  Ogre::SceneManager* sceneManager_; ///< Non-owning pointer
  Ogre::Camera* camera_;             ///< Non-owning pointer
};

} // namespace game

} // namespace sequoia

#endif
