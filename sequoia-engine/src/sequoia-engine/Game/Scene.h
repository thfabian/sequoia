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

#ifndef SEQUOIA_ENGINE_GAME_SCENE_H
#define SEQUOIA_ENGINE_GAME_SCENE_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Listenable.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Game/GameFwd.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include <memory>
#include <string>
#include <vector>

namespace sequoia {

namespace game {

/// @brief Listen to changes in the state of the Scene
/// @ingroup game
class SEQUOIA_API SceneListener {
public:
  virtual ~SceneListener() {}

  /// @brief The active camera of the scene changed
  virtual void sceneListenerActiveCameraChanged(Scene* scene) = 0;
};

/// @brief A scene contains all the necessary objects to progess the game forward in time
/// @ingroup game
class SEQUOIA_API Scene : public Listenable<SceneListener>, public NonCopyable {
public:
  friend class Game;

  /// @brief Create the empty scene `name`
  Scene(const std::string& name);

  /// @brief Destruct scene
  virtual ~Scene() {}

  /// @brief Set the active Camera
  void setActiveCamera(const std::shared_ptr<render::Camera>& camera);

  /// @brief Get the active Camera
  const std::shared_ptr<render::Camera>& getActiveCamera() const;

  /// @brief Get the SceneGraph
  SceneGraph* getSceneGraph() const;

  /// @brief Get the name of the Scene
  const std::string& getName() const { return name_; }

  /// @brief Update the scene and progress to the next time-step
  ///
  /// By default, this does nothing.
  virtual void update();

  /// @name Rendering
  /// @{

  /// @brief Prepare a list of `RenderTechnique`s which is used in the next render call
  ///
  /// This is called automatically by `prepareRenderCommand`. By default this does nothing.
  virtual void prepareRenderTechniques(std::vector<render::RenderTechnique*>& techiques);

  /// @brief Prepare a list of `DrawCommand`s which is used in the next render call
  ///
  /// This is called automatically by `prepareRenderCommand`. By default the SceneGraph is traversed
  /// and each actice `Drawable` is added to the `drawCommands`.
  virtual void prepareDrawCommands(std::vector<render::DrawCommand>& drawCommands);

  /// @brief Prepare the RenderTarget whis is used in the next render call
  ///
  /// This is called automatically by `prepareRenderCommand`. By default, this does not change the
  /// `target` and thus retains the the default RenderTarget of the Game (which is the screen).
  virtual void prepareRenderTarget(render::RenderTarget*& target);

  /// @}

private:
  /// @brief Send an update notification to all SceneNodes in the SceneGraph and call `update()`
  ///
  /// This is called by the `Game` in the main-loop.
  void updateImpl(float timeStep);

  /// @brief Prepare the RenderCommand by calling `prepareRenderTechniques` as well as
  /// `prepareDrawCommands`
  ///
  /// This is called by the `Game` in the main-loop.
  void prepareRenderCommand(render::RenderCommand& cmd);

private:
  /// Name of the scene
  std::string name_;

  /// Graph of the scene
  std::shared_ptr<SceneGraph> sceneGraph_;

  /// Currently active camera
  std::shared_ptr<render::Camera> activeCamera_;
};

} // namespace game

} // namespace sequoia

#endif
