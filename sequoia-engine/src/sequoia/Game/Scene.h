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

#ifndef SEQUOIA_GAME_SCENE_H
#define SEQUOIA_GAME_SCENE_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Game/Export.h"
#include "sequoia/Render/RenderFwd.h"
#include <memory>
#include <vector>

namespace sequoia {

namespace game {

class SceneGraph;

/// @brief State of a scene
/// @ingroup game
class SEQUOIA_GAME_API Scene : public NonCopyable {

  /// List of currently active DrawCommands of this scene
  std::vector<render::DrawCommand*> drawCommandList_;

  /// Graph of the scene
  std::shared_ptr<SceneGraph> sceneGraph_;

  /// Currently active camera
  std::shared_ptr<render::Camera> activeCamera_;

public:
  /// @brief Create an empty scene
  Scene();

  /// @brief Update the DrawCommandList with the current scene
  void updateDrawCommandList(render::DrawCommandList* list);

  /// @brief Set the active Camera
  void setActiveCamera(const std::shared_ptr<render::Camera>& camera);
  
  /// @brief Get the active Camera
  const std::shared_ptr<render::Camera>& getActiveCamera() const;

  /// @brief Get the SceneGraph
  SceneGraph* getSceneGraph() const;
};

} // namespace game

} // namespace sequoia

#endif
