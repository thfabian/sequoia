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

#ifndef SEQUOIA_ENGINE_GAME_DRAWABLE_H
#define SEQUOIA_ENGINE_GAME_DRAWABLE_H

#include "sequoia-engine/Game/GameFwd.h"
#include "sequoia-engine/Game/SceneNodeCapability.h"
#include "sequoia-engine/Game/Shape.h"
#include "sequoia-engine/Render/DrawCommand.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include <memory>

namespace sequoia {

namespace game {

/// @brief Add the capability to a SceneNode to be drawn to the screen
/// @ingroup game
class SEQUOIA_API Drawable final : public SceneNodeCapability {
  /// Is drawing enabled?
  bool active_;

  /// Shape of the node (including the mesh and material properties)
  std::shared_ptr<Shape> shape_;

public:
  using Base = SceneNodeCapability;

  /// @brief Virtual destructor
  virtual ~Drawable();

  /// @brief Construct with the associated shape
  Drawable(SceneNode* node, const std::shared_ptr<Shape>& shape);

  /// @brief Is the node rendered?
  bool isActive() const { return active_; }

  /// @brief Set if the node is rendered
  void setActive(bool active) { active_ = active; }

  /// @brief Get the associated shape
  Shape* getShape() noexcept { return shape_.get(); }
  const Shape* getShape() const noexcept { return shape_.get(); }

  /// @brief Prepare the DrawCommand for rendering
  void prepareDrawCommands(std::vector<render::DrawCommand>& drawCommands);

  /// @copydoc SceneNodeCapability::update
  virtual void update(const SceneNodeUpdateEvent& event) override;

  /// @copydoc SceneNodeCapability::toString
  virtual std::string toString() const override;

  /// @copydoc SceneNodeCapability::clone
  virtual std::shared_ptr<SceneNodeCapability> clone(SceneNode* node) const override;
};

SEQUOIA_REGISTER_SCENENODE_CAPABILITY(Drawable)

} // namespace game

} // namespace sequoia

#endif
