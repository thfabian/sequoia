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

#include "sequoia-engine/Core/DoubleBuffered.h"
#include "sequoia-engine/Game/GameFwd.h"
#include "sequoia-engine/Game/SceneNodeCapability.h"
#include "sequoia-engine/Render/DrawCommand.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <memory>
#include <unordered_map>

namespace sequoia {

namespace game {

/// @brief Add the capability to a SceneNode to be drawn to the screen
/// @ingroup game
class SEQUOIA_API Drawable final : public SceneNodeCapability {
  /// Is drawing enabled?
  bool active_;

  /// DrawCommand of the mesh
  DoubleBuffered<render::DrawCommand> drawCommand_;

  /// Mesh of the node
  std::shared_ptr<Mesh> mesh_;

  //  /// Shape of the node (including the mesh and material properties)
  //  std::shared_ptr<Shape> shape_;

public:
  using Base = SceneNodeCapability;

  /// @brief Virtual destructor
  virtual ~Drawable();

  /// @brief Construct with the associated shape
  Drawable(SceneNode* node, const std::shared_ptr<Mesh>& mesh);

  // TODO: remove, this information is passed in via the Material -----------

  /// @brief Get the Mesh of the node
  const std::shared_ptr<Mesh>& getMesh() const { return mesh_; }

  /// @brief Set the Mesh of the node
  ///
  /// This also sets the `DrawCommand` if the mesh contains a valid `DrawCommand`.
  void setMesh(const std::shared_ptr<Mesh>& mesh);

  /// @brief Check if the node has a Mesh attached
  bool hasMesh() const { return mesh_ != nullptr; }

  /// @copydoc sequoia::render::DrawCommand::setTexture
  void setTexture(int textureUnit, render::Texture* texture) noexcept;

  // -----------------------

  /// @brief Set a uniform variable
  void setUniform(const std::string& name, render::UniformVariable variable);

  /// @brief Is the node rendered?
  bool isActive() const { return active_; }

  /// @brief Set if the node is rendered
  void setActive(bool active) { active_ = active; }

  /// @brief Prepare the DrawCommand for rendering
  ///
  /// @note This copies the model-matrix and advances the underlying uniform and vertex-data buffers
  /// to the next timestep. The values can be modified freely afterwards without corrupting the
  /// rendering.
  render::DrawCommand* prepareDrawCommand();

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
