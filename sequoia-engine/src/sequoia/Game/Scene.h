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

#ifndef SEQUOIA_GAME_SCENE_H
#define SEQUOIA_GAME_SCENE_H

#include "sequoia/Core/ConcurrentADT.h"
#include "sequoia/Core/DoubleBuffered.h"
#include "sequoia/Core/Export.h"
#include "sequoia/Core/Listenable.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Game/Emittable.h"
#include "sequoia/Game/GameFwd.h"
#include "sequoia/Render/RenderCommand.h"
#include "sequoia/Render/RenderFwd.h"
#include <array>
#include <atomic>
#include <memory>
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

  /// @brief Create an empty scene
  Scene();

  /// @brief Destruct scene
  virtual ~Scene() {}

  /// @brief Set the active Camera
  void setActiveCamera(const std::shared_ptr<render::Camera>& camera);

  /// @brief Get the active Camera
  const std::shared_ptr<render::Camera>& getActiveCamera() const;

  /// @brief Get the SceneGraph
  SceneGraph* getSceneGraph() const;

  /// @brief Prepare the RenderCommand for rendering the next frame to `target`
  render::RenderCommand* prepareRenderCommand(render::RenderTarget* target) noexcept;

  /// @brief Update the scene and progress to the next time-step
  ///
  /// By default, this does nothing.
  virtual void update();

  // TODO: remove me
  /// @brief Create a dummy test scene
  void makeDummyScene();

private:
  /// @brief Calls `update()` followed by an update notification to all SceneNodes in the SceneGraph
  ///
  /// This is called by the `Game` in the main-loop.
  void updateImpl();

  /// @brief Populate the RenderCommand by extracting DrawCommands and setting the GlobalRenderState
  void populateRenderCommand(render::RenderCommand* command);

private:
  /// Double buffered render command
  DoubleBuffered<render::RenderCommand> renderCommand_;

  /// Graph of the scene
  std::shared_ptr<SceneGraph> sceneGraph_;

  /// Currently active camera
  std::shared_ptr<render::Camera> activeCamera_;

  /// Temporary list of draw-commands
  concurrent_vector<render::DrawCommand*> drawCommands_;

  /// Temporary list of emitters
  std::array<std::atomic<int>, Emittable::NumEmitter> emitters_;
};

} // namespace game

} // namespace sequoia

#endif
