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

#ifndef SEQUOIA_GAME_SCENENODE_H
#define SEQUOIA_GAME_SCENENODE_H

#include "sequoia/Game/Export.h"
#include "sequoia/Math/Math.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace sequoia {

namespace game {

/// @brief Base class of all scene nodes in the SceneGraph
/// @ingroup game
class SEQUOIA_GAME_API SceneNode : public std::enable_shared_from_this<SceneNode> {
public:
  enum SceneNodeKind { SK_Static };

  /// @brief Default constructor
  SceneNode(SceneNodeKind kind, const std::shared_ptr<SceneNode>& parent, const std::string& name,
            const math::vec3& position = math::vec3(), const math::quat& orientation = math::quat(),
            float scale = 1.0f);

  virtual ~SceneNode();

  /// @name Getter/Setter
  /// @{

  /// @brief Get the name of the node
  const std::string& getName() const { return name_; }

  /// @brief Get the name of the node
  void setName(const std::string& name) { name_ = name; }

  /// @brief Get the position (in world space
  const math::vec3& getPosition() const { return position_; }

  /// @brief Set the position (in world space)
  void setPosition(const math::vec3& position) { position_ = position; }

  /// @brief Get the orientation
  const math::quat& getOrientation() const { return orientation_; }

  /// @brief Set the orientation (in world space)
  void setOrientation(const math::quat& orientation) { orientation_ = orientation; }

  /// @brief Get the scaling factor
  float getScale() const { return scale_; }

  /// @brief Set the scaling factor
  void setScale(float scale) { scale_ = scale; }

  /// @brief Add a child to the scene node
  void addChild(const std::shared_ptr<SceneNode>& child) { children_.emplace_back(child); }

  /// @brief Check if the child has any children
  bool hasChildren() const { return !children_.empty(); }

  /// @brief Get the children
  const std::vector<std::shared_ptr<SceneNode>>& getChildren() const { return children_; }

  /// @brief Check if this node is attached to a parent node
  bool hasParent() { return !parent_.expired(); }

  /// @brief Get the parent node
  /// @returns parent node or `NULL` if no parent is available
  std::shared_ptr<SceneNode> getParent() const { return parent_.lock(); }

  /// @brief Set the parent node
  void setParent(const std::shared_ptr<SceneNode>& parent) { parent_ = parent; }

  /// @}

  /// @name Streaming operations
  /// @{

  /// @brief Apply `functor` to the node and all its children
  void apply(const std::function<void(SceneNode*)>& functor);

  /// @}

  /// @name Miscellaneous
  /// @{

  /// @brief Clone the scene node and all its children
  virtual std::shared_ptr<SceneNode> clone() = 0;

  /// @brief Convert the node to string
  virtual std::string toString() const = 0;

  /// @brief Get the kind of the ScenenNode
  SceneNodeKind getKind() const { return kind_; }

  /// @}

private:
  SceneNodeKind kind_;

  /// List of children
  std::vector<std::shared_ptr<SceneNode>> children_;

  /// Parent node (if any)
  std::weak_ptr<SceneNode> parent_;

  /// Position of the node
  math::vec3 position_;

  /// Orientation of the node
  math::quat orientation_;

  /// Scaling factor
  float scale_;

  /// Name of the node
  std::string name_;
};

} // namespace game

} // namespace sequoia

#endif
