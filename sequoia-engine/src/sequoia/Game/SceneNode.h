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

#ifndef SEQUOIA_GAME_SCENENODE_H
#define SEQUOIA_GAME_SCENENODE_H

#include "sequoia/Core/Export.h"
#include "sequoia/Game/SceneNodeAlloc.h"
#include "sequoia/Game/SceneNodeCapability.h"
#include "sequoia/Math/Math.h"
#include <array>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace sequoia {

namespace game {

/// @brief Event indicating the scenen has progressed to the next time-step
struct SceneNodeUpdateEvent {
  /// Time since last update (i.e time-step)
  float TimeStep;
};

/// @brief Base class of all scene nodes in the SceneGraph
/// @ingroup game
class SEQUOIA_API SceneNode : public std::enable_shared_from_this<SceneNode> {
public:
  using UpdateEvent = SceneNodeUpdateEvent;

  /// @brief Create a SceneNode of type `T` by passing `args...` to the constructor
  template <class T = SceneNode, class... Args>
  static std::shared_ptr<T> create(Args&&... args) {
    static_assert(std::is_base_of<SceneNode, T>::value, "type 'T' is not a SceneNode");
    return SceneNodeAlloc::create<T>(std::forward<Args>(args)...);
  }

  /// @brief RTTI discriminator
  enum SceneNodeKind {
    SK_SceneNode,
    SK_CameraController,
    SK_CameraControllerFree,
    SK_CameraControllerLast
  };

  /// @brief Enumeration denoting the spaces which a transform can be relative to
  enum TransformSpace {
    TS_Local,  ///< Transform is relative to the local space
    TS_Parent, ///< Transform is relative to the space of the parent node
    TS_World   ///< Transform is relative to world space
  };

  SceneNode(const std::string& name, SceneNodeKind kind = SK_SceneNode);
  SceneNode(const SceneNode& other);

  /// @brief Virtual destructor
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
  void setPosition(const math::vec3& position) {
    modelMatrixIsDirty_ = true;
    position_ = position;
  }

  /// @brief Get the orientation
  const math::quat& getOrientation() const { return orientation_; }

  /// @brief Set the orientation (in world space)
  void setOrientation(const math::quat& orientation) {
    modelMatrixIsDirty_ = true;
    orientation_ = orientation;
  }

  /// @brief Resets the orientation (local axes as world axes, no rotation)
  void resetOrientation();

  /// @brief Get the scaling factor
  float getScale() const { return scale_; }

  /// @brief Set the scaling factor
  virtual void setScale(float scale) {
    modelMatrixIsDirty_ = true;
    scale_ = scale;
  }

  /// @brief Get a matrix whose columns are the local axes based on the nodes orientation
  /// relative to it's parent
  math::mat3 getLocalAxes() const;

  /// @brief Get the model matrix
  const math::mat4& getModelMatrix() {
    if(modelMatrixIsDirty_)
      computeModelMatrix();
    return modelMatrix_;
  }

  /// @brief Add a child to the scene node
  void addChild(const std::shared_ptr<SceneNode>& child) {
    children_.emplace_back(child);
    children_.back()->setParent(shared_from_this());
  }

  /// @brief Check if the child has any children
  bool hasChildren() const { return !children_.empty(); }

  /// @brief Get the children
  const std::vector<std::shared_ptr<SceneNode>>& getChildren() const { return children_; }

  /// @brief Check if this node is attached to a parent node
  bool hasParent() const { return !parent_.expired(); }

  /// @brief Get the parent node
  /// @returns parent node or `NULL` if no parent is available
  std::shared_ptr<SceneNode> getParent() const { return parent_.lock(); }

  /// @brief Set the parent node
  void setParent(const std::shared_ptr<SceneNode>& parent) { parent_ = parent; }

  /// @}
  /// @name Operations
  /// @{

  /// @brief Apply `functor` to the node and all its children
  void apply(const std::function<void(SceneNode*)>& functor);

  /// @brief Update the node to indicate the scene progressed to the next time-step
  virtual void update(const UpdateEvent& event);

  /// @brief Translate the node by `d`
  virtual void translate(const math::vec3& d, TransformSpace relativeTo = TS_Parent);

  /// @brief Rotate the node by `q`
  virtual void rotate(const math::quat& q, TransformSpace relativeTo = TS_Parent);

  /// @brief Rotate the node by `angle` around `axis`
  ///
  /// @param axis   Axis to rotate around
  /// @param angle  Angle in radians
  void rotate(const math::vec3& axis, const math::Radian& angle,
              TransformSpace relativeTo = TS_Parent);

  /// @brief Rotate the node by `angle` around X-axis
  ///
  /// @param angle  Angle in radians
  void pitch(const math::Radian& angle, TransformSpace relativeTo = TS_Parent);

  /// @brief Rotate the node by `angle` around Y-axis
  ///
  /// @param angle  Angle in radians
  void yaw(const math::Radian& angle, TransformSpace relativeTo = TS_Parent);

  /// @brief Rotate the node by `angle` around Z-axis
  ///
  /// @param angle  Angle in radians
  void roll(const math::Radian& angle, TransformSpace relativeTo = TS_Parent);

  /// @}
  /// @name Capabilities
  /// @{

  /// @brief Add a new SceneNodeCapability of type `T` by passing `args...` to the constructor
  ///
  /// @note The first argument of the constructor, which is the pointer to the respective SceneNode,
  /// is added automatically.
  template <class T, class... Args>
  void addCapability(Args&&... args) {
    capabilities_[SceneNodeCapabilityToKind<T>::value] =
        SceneNodeAlloc::create<T>(this, std::forward<Args>(args)...);
  }

  /// @brief Check if the SceneNode poses the given SceneNodeCapability
  /// @returns `true` if capability is available, `false` on absence
  template <class T>
  inline bool is() const noexcept {
    return capabilities_[SceneNodeCapabilityToKind<T>::value] != nullptr;
  }

  /// @brief Get the SceneNodeCapability of type `T`
  /// @returns pointer to the capability or `NULL` if capability is absence
  template <class T>
  inline const T* get() const noexcept {
    return static_cast<const T*>(capabilities_[SceneNodeCapabilityToKind<T>::value].get());
  }
  template <class T>
  inline T* get() noexcept {
    return static_cast<T*>(capabilities_[SceneNodeCapabilityToKind<T>::value].get());
  }

  /// @}
  /// @name Miscallenous
  /// @{

  /// @brief Clone the scene node and all its children
  virtual std::shared_ptr<SceneNode> clone();

  /// @brief Convert the node to string
  std::string toString() const;

  /// @brief Get the kind of the ScenenNode
  SceneNodeKind getKind() const { return kind_; }

  /// @}

  /// @brief RTTI implementation
  static bool classof(const SceneNode* node) noexcept { return node->getKind() == SK_SceneNode; }

protected:
  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const;

  /// @brief Compute the model matrix
  void computeModelMatrix();

private:
  /// Type of node
  SceneNodeKind kind_;

  /// Capabilities of the node (nullptr indicates absence of the capability)
  std::array<std::shared_ptr<SceneNodeCapability>, SceneNodeCapability::NumCapabilities>
      capabilities_;

  /// List of children
  std::vector<std::shared_ptr<SceneNode>> children_;

  /// Position of the node relative to it's parent.
  math::vec3 position_;

  /// Orientation of the node relative to it's parent.
  math::quat orientation_;

  /// Scaling factor
  float scale_;

  /// ModelView matrix
  math::mat4 modelMatrix_;

  /// ModelView matrix needs to be recomputed
  bool modelMatrixIsDirty_;

  /// Parent node (if any)
  std::weak_ptr<SceneNode> parent_;

  /// Name of the node
  std::string name_;
};

} // namespace game

} // namespace sequoia

#endif
