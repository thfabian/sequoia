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

#ifndef SEQUOIA_GAME_SCENENODEDRAWABLE_H
#define SEQUOIA_GAME_SCENENODEDRAWABLE_H

#include "sequoia/Game/SceneNode.h"
#include "sequoia/Render/RenderFwd.h"

namespace sequoia {

namespace game {

class Mesh;

/// @brief SceneNode which can be drawn
/// @ingroup game
class SEQUOIA_GAME_API SceneNodeDrawable : public SceneNode {
public:
  using Base = SceneNode;

  SceneNodeDrawable(const std::string& name);
  SceneNodeDrawable(const SceneNodeDrawable& other);

  virtual ~SceneNodeDrawable();

  /// @name Getter/Setter
  /// @{

  /// @brief Get the Mesh of the node
  const std::shared_ptr<Mesh>& getMesh() const { return mesh_; }

  /// @brief Set the Mesh of the node
  ///
  /// This also sets the `DrawCommand` if the mesh contains a valid `DrawCommand`.
  void setMesh(const std::shared_ptr<Mesh>& mesh);

  /// @brief Check if the node has a Mesh attached
  bool hasMesh() const { return mesh_ != nullptr; }

  /// @brief Get the DrawCommand of the node
  const std::shared_ptr<render::DrawCommand>& getDrawCommand() const { return drawCommand_; }

  /// @brief Set the Program used in the render-pipeline when invoking the `DrawCommand`
  void setProgram(const std::shared_ptr<render::Program>& program);

  /// @brief Prepare the DrawCommand for rendering
  ///
  /// @note
  /// This copies the model-matrix and the render-state to an internal queue and the values can be
  /// be modified freely afterwards without corrupting the rendering.
  render::DrawCommand* prepareDrawCommand();

  /// @}

  /// @name Miscellaneous
  /// @{

  /// @brief Clone the scene node and all its children
  virtual std::shared_ptr<SceneNode> clone() override;

  /// @brief RTTI implementation
  static bool classof(const SceneNode* node) noexcept {
    return node->getKind() == SK_SceneNodeDrawable || node->getKind() < SK_SceneNodeDrawableLast;
  }
  /// @}

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const override;

protected:
  /// DrawCommand of the mesh
  std::shared_ptr<render::DrawCommand> drawCommand_;

  /// Mesh of the node
  std::shared_ptr<Mesh> mesh_;
};

} // namespace game

} // namespace sequoia

#endif
