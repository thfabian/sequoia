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
class AssetManager;
class Scene;

/// @brief Main class holding all game and rendering related objects and running the main-loop
/// @ingroup game
class SEQUOIA_GAME_API Game : public Singleton<Game>,
                              public KeyListener,
                              public MouseListener,
                              public Listenable<KeyListener, MouseListener> {

  /// Active render-system
  std::unique_ptr<render::RenderSystem> renderSystem_;

  /// Manager
  std::unique_ptr<MeshManager> meshManager_;
  std::unique_ptr<AssetManager> assetManager_;

  /// Reference to the main window
  render::RenderWindow* mainWindow_;

  /// List of scenes
  std::vector<std::shared_ptr<Scene>> sceneList_;

  /// Currently active scene
  Scene* scene_;

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

  /// @brief Get the mesh manager
  MeshManager* getMeshManager() const;

  /// @brief Get the asset manager
  AssetManager* getAssetManager() const;

  /// @brief Get the main RenderWindow
  render::RenderWindow* getMainWindow() const;

  /// @brief Get the main RenderTarget
  render::RenderTarget* getMainRenderTarget() const;

  /// @brief Get the default vertex shader
  const std::shared_ptr<render::Shader>& getDefaultVertexShader() const;

  /// @brief Get the default fragment shader
  const std::shared_ptr<render::Shader>& getDefaultFragmentShader() const;

  /// @brief Get the default program
  const std::shared_ptr<render::Program>& getDefaultProgram() const;

  /// @brief Get the main Scene
  Scene* getScene() const;

  /// @brief Listener implementations
  /// @{
  void mouseButtonEvent(const render::MouseButtonEvent& event) override;
  void mousePositionEvent(const render::MousePositionEvent& event) override;
  void keyboardEvent(const render::KeyboardEvent& event) override;
  /// @}
};

} // namespace game

} // namespace sequoia

#endif
