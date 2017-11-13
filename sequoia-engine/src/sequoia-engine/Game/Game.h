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

#ifndef SEQUOIA_ENGINE_GAME_GAME_H
#define SEQUOIA_ENGINE_GAME_GAME_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Image.h"
#include "sequoia-engine/Core/Listenable.h"
#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/Singleton.h"
#include "sequoia-engine/Game/GameFwd.h"
#include "sequoia-engine/Game/Scene.h"
#include "sequoia-engine/Render/Input.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/Shader.h"
#include <initializer_list>
#include <memory>
#include <set>
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

  /// Managers
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

  /// Options of the game
  std::shared_ptr<Options> options_;

public:
  Game();

  /// @brief Cleanup all allocted objects
  ~Game();

  /// @brief Create the Options with default values for the sequoia-engine
  static std::shared_ptr<Options> makeOptions();

  /// @brief Initialize the game with `gameOptions`
  ///
  /// To construct the options, use `Game::makeOptions()`.
  void init(const std::shared_ptr<Options>& gameOptions = Game::makeOptions());

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

  /// @brief Get the RenderSystem
  render::RenderSystem* getRenderSystem() const { return renderSystem_.get(); }

  /// @brief Create a texture from the `image` using default parameters
  render::Texture* createTexture(const std::shared_ptr<Image>& image);

  /// @brief Create a texture from the `image` and `param`
  render::Texture* createTexture(const std::shared_ptr<Image>& image,
                                 const render::TextureParameter& param);

  /// @brief Compile the given shader
  std::shared_ptr<render::Shader> createShader(render::Shader::ShaderType type,
                                               const std::shared_ptr<File>& source);

  /// @brief Create a GPU program by compiling the given shaders sources
  std::shared_ptr<render::Program> createProgram(
      std::initializer_list<std::pair<render::Shader::ShaderType, std::shared_ptr<File>>> shaders);

  /// @brief Create a GPU program of the given shaders
  std::shared_ptr<render::Program>
  createProgram(const std::set<std::shared_ptr<render::Shader>>& shaders);

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

  void mouseButtonEvent(const render::MouseButtonEvent& event) override;
  void mousePositionEvent(const render::MousePositionEvent& event) override;
  void keyboardEvent(const render::KeyboardEvent& event) override;

private:
  static void setDefaultOptions(const std::shared_ptr<Options>& options);
};

} // namespace game

} // namespace sequoia

#endif
