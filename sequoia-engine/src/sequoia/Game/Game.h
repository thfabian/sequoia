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

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Image.h"
#include "sequoia/Core/Listenable.h"
#include "sequoia/Core/Singleton.h"
#include "sequoia/Game/GameFwd.h"
#include "sequoia/Game/Scene.h"
#include "sequoia/Render/Input.h"
#include "sequoia/Render/RenderFwd.h"
#include <memory>
#include <unordered_map>

namespace sequoia {

namespace game {

/// @brief Main class holding all game and rendering related objects and running the main-loop
/// @ingroup game
class SEQUOIA_API Game final : public Singleton<Game>,
                               public KeyListener,
                               public MouseListener,
                               public SceneListener,
                               public Listenable<KeyListener, MouseListener> {

  /// Active render-system
  std::unique_ptr<render::RenderSystem> renderSystem_;

  /// Manager
  std::unique_ptr<AssetManager> assetManager_;
  std::unique_ptr<MeshManager> meshManager_;

  /// Reference to the main window
  render::RenderWindow* mainWindow_;

  /// Shortcut key to quit the game
  std::shared_ptr<Keymap> quitKey_;
  bool shouldClose_;

  /// List of scenes
  std::unordered_map<std::string, std::shared_ptr<Scene>> sceneMap_;

  /// Currently active scene
  Scene* activeScene_;

  /// Name of the game
  std::string name_;

public:
  Game(std::string name = "Game");

  /// @brief Cleanup all allocted objects
  ~Game();

  /// @brief Initializes the game object by initializing the RenderSystem
  /// @param hideWindow   Hide the main-window
  void init(bool hideWindow = false);

  /// @brief Frees all allocated resources
  void cleanup();

  /// @brief Run the main-loop
  void run();

  /// @brief Set the quit key (use `nullptr` to disable the quit key)
  void setQuitKey(const std::shared_ptr<Keymap>& key);

  /// @brief Get the asset manager
  AssetManager* getAssetManager() const;

  /// @brief Get the mesh manager
  MeshManager* getMeshManager() const;

  /// @brief Get the main RenderWindow
  render::RenderWindow* getMainWindow() const;

  /// @brief Get the main RenderTarget
  render::RenderTarget* getMainRenderTarget() const;

  /// @brief Create a texture from the `image` using default parameters
  render::Texture* createTexture(const std::shared_ptr<Image>& image);

  /// @brief Create a texture from the `image` and `param`
  render::Texture* createTexture(const std::shared_ptr<Image>& image,
                                 const render::TextureParameter& param);

  /// @brief Get the default vertex shader
  const std::shared_ptr<render::Shader>& getDefaultVertexShader() const;

  /// @brief Get the default fragment shader
  const std::shared_ptr<render::Shader>& getDefaultFragmentShader() const;

  /// @brief Get the default program
  const std::shared_ptr<render::Program>& getDefaultProgram() const;

  /// @name Scene operations
  /// @{

  /// @brief Get the currently active scene
  Scene* getActiveScene() const;

  /// @brief Append a new scene `name`
  void setScene(const std::string& name, const std::shared_ptr<Scene>& scene,
                bool makeActive = true);

  /// @brief Get the scene `name`
  Scene* getScene(const std::string& name) const;

  /// @brief Remove the scene `name`
  void removeScene(const std::string& name);

  /// @brief Make the scene `name` the currently active one
  void makeSceneActive(const std::string& name);

  /// @brief Listen to the currently active scene
  void sceneListenerActiveCameraChanged(Scene* scene) override;

  /// @}

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
