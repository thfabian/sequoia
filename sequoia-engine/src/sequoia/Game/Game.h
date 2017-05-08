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

#ifndef SEQUOIA_GAME_GAME_H
#define SEQUOIA_GAME_GAME_H

#include "sequoia/Core/Singleton.h"
#include "sequoia/Game/Export.h"
#include "sequoia/Render/RenderFwd.h"
#include <memory>

namespace sequoia {

namespace game {

/// @brief Main class holding all game and rendering related objects and running the main-loop
/// @ingroup game
class SEQUOIA_GAME_API Game : public Singleton<Game> {
  
  /// Active render-system
  std::shared_ptr<render::RenderSystem> renderSystem_;

  /// Reference to the main window
  render::RenderWindow* mainWindow_;

  std::shared_ptr<render::Camera> mainCamera_;

public:
  /// @copydoc initialize
  Game();

  /// @copydoc cleanup
  ~Game();

  /// @brief Initializes the game object by initializing the RenderSystem
  void init();

  /// @brief Frees all allocated resources
  void cleanup();

  /// @brief Run the main-loop
  void run();
};
} // namespace game

} // namespace sequoia

#endif
