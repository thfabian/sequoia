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

#ifndef SEQUOIA_GAME_DRAWABLE_H
#define SEQUOIA_GAME_DRAWABLE_H

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

public:
  using Base = SceneNodeCapability;

  /// @brief Virtual destructor
  virtual ~Drawable();

  /// @brief Construct with mesh and GPU porgram
  ///
  /// @param mesh       Mesh to use
  /// @param program    GPU program to use, if `NULL` the default program of the game will be used
  ///                   (Game::getDefaultProgram())
  Drawable(SceneNode* node, const std::shared_ptr<Mesh>& mesh, render::Program* program = nullptr);
  Drawable(SceneNode* node, const std::shared_ptr<Mesh>& mesh,
           const std::shared_ptr<render::Program>& program);

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

  /// @brief Get the DrawCommand of the node
  const render::DrawCommand& getDrawCommand() const { return drawCommand_.get(); }
  render::DrawCommand& getDrawCommand() { return drawCommand_.get(); }

  /// @brief Get the RenderState of the DrawCommand
  render::RenderState& getRenderState();
  const render::RenderState& getRenderState() const;

  /// @brief Set the Program used in the render-pipeline when invoking the `DrawCommand`
  void setProgram(render::Program* program) noexcept;
  void setProgram(const std::shared_ptr<render::Program>& program) noexcept {
    setProgram(program.get());
  }

  /// @brief Set a uniform variable
  template <class T>
  void setUniformVariable(const std::string& name, const T& variable) {
    setUniformVariableImpl(name, variable);
  }

  /// @brief Is the node rendered?
  bool isActive() const { return active_; }

  /// @brief Set if the node is rendered
  void setActive(bool active) { active_ = active; }

  /// @brief Prepare the DrawCommand for rendering
  ///
  /// @note This copies the model-matrix and the render-state to an internal queue. The values
  /// can be modified freely afterwards without corrupting the rendering.
  render::DrawCommand* prepareDrawCommand();

  /// @copydoc SceneNodeCapability::update
  virtual void update(const SceneNodeUpdateEvent& event) override;

  /// @copydoc SceneNodeCapability::toString
  virtual std::string toString() const override;

  /// @copydoc SceneNodeCapability::clone
  virtual std::shared_ptr<SceneNodeCapability> clone(SceneNode* node) const override;

private:
  void setUniformVariableImpl(const std::string& name, UniformVariable variable);
};

SEQUOIA_REGISTER_SCENENODE_CAPABILITY(Drawable)

} // namespace game

} // namespace sequoia

#endif
