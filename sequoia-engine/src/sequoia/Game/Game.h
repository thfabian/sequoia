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

#include "sequoia/Core/Listenable.h"
#include "sequoia/Core/Singleton.h"
#include "sequoia/Game/Export.h"
#include "sequoia/Render/Input.h"
#include "sequoia/Render/RenderFwd.h"
#include <memory>

namespace sequoia {

namespace game {

class MeshManager;

/// @brief Main class holding all game and rendering related objects and running the main-loop
/// @ingroup game
class SEQUOIA_GAME_API Game : public Singleton<Game>,
                              public KeyListener,
                              public MouseListener,
                              public Listenable<KeyListener, MouseListener> {

  /// Active render-system
  std::unique_ptr<render::RenderSystem> renderSystem_;

  /// Active mesh manager
  std::unique_ptr<MeshManager> meshManager_;

  /// Reference to the main window
  render::RenderWindow* mainWindow_;

  // TODO: should be part of the scene
  std::unique_ptr<render::Camera> mainCamera_;

public:
  Game();

  /// @brief Cleanup all allocted objects
  ~Game();
  
  /// @brief Initializes the game object by initializing the RenderSystem
  /// @param hideWindow   Hide the main-window
  void init(bool hideWindow = false);

  /// @brief Frees all allocated resources
  void cleanup();

  /// @brief Run the main-loop
  void run();

  /// @brief Listener implementations
  /// @{
  void mouseEvent(const render::MouseEvent& event) override;
  void keyboardEvent(const render::KeyboardEvent& event) override;
  /// @}

  /// @brief Get the mesh manager
  MeshManager* getMeshManager();
};

} // namespace game

} // namespace sequoia

#endif
