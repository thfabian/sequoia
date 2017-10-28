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

#ifndef SEQUOIA_ENGINE_GAME_SCENENODECAPABILITY_H
#define SEQUOIA_ENGINE_GAME_SCENENODECAPABILITY_H

#include "sequoia-engine/Core/Export.h"
#include <memory>
#include <string>

namespace sequoia {

namespace game {

class SceneNode;
struct SceneNodeUpdateEvent;

/// @brief Attach capabilities to scene nodes like Drawable, Animable or Soundable
/// @ingroup game
class SEQUOIA_API SceneNodeCapability {
  SceneNode* node_;

public:
  /// @brief Default constructor
  ///
  /// @note All subclasses are required to have `SceneNode* node` as their first argument in their
  /// constructors to be compatible with
  /// @ref sequoia::game::SceneNode::addCapability "SceneNode::addCapability"
  SceneNodeCapability(SceneNode* node) : node_(node) {}

  /// @brief Enum for distinguishing capabilities
  /// @note The name of the enum needs to correspond to the type name of the SceneNodeCapability
  enum Kind {
    Drawable = 0, ///< The SceneNode can be rendered
    Emittable,    ///< The SceneNode emits some form of lighting
    NumCapabilities
  };

  /// @brief Virtual destructor
  virtual ~SceneNodeCapability() {}

  /// @brief Get the attached SceneNode
  SceneNode* getNode() { return node_; }
  const SceneNode* getNode() const { return node_; }

  /// @brief Set the SceneNode
  void setNode(SceneNode* node) { node_ = node; }

  /// @brief Update the node to indicate the scene progressed to the next time-step
  virtual void update(const SceneNodeUpdateEvent& event) = 0;

  /// @brief Convert to string
  virtual std::string toString() const = 0;

  /// @brief Clone the capability using `node` as the new attached SceneNode
  virtual std::shared_ptr<SceneNodeCapability> clone(SceneNode* node) const = 0;
};

namespace internal {

template <SceneNodeCapability::Kind Kind>
struct InvalidCapabilityType {
  using type = void;
};

template <class T>
struct InvalidCapabilityKind {
  static constexpr SceneNodeCapability::Kind value = SceneNodeCapability::NumCapabilities;
};

} // namespace internal

template <SceneNodeCapability::Kind Kind>
struct SceneNodeCapabilityToType {
  using type = typename internal::InvalidCapabilityType<Kind>::type;
  static_assert(!std::is_same<type, void>::value, "invalid SceneNodeCapability type");
};

template <class T>
struct SceneNodeCapabilityToKind {
  static constexpr SceneNodeCapability::Kind value = internal::InvalidCapabilityKind<T>::value;
  static_assert(value != SceneNodeCapability::NumCapabilities, "invalid SceneNodeCapability::Kind");
};

/// @brief Register a new SceneNodeCapability
/// @param TypeName   Type of the @ref sequoia::game::SceneNodeCapability "SceneNodeCapability".
///                   Note that an enum with the same name needs to exist in
///                   @ref sequoia::game::SceneNodeCapability::Kind "SceneNodeCapability::Kind"
/// @ingroup game
#define SEQUOIA_REGISTER_SCENENODE_CAPABILITY(TypeName)                                            \
  template <>                                                                                      \
  struct SceneNodeCapabilityToType<SceneNodeCapability::TypeName> {                                \
    using type = TypeName;                                                                         \
  };                                                                                               \
  template <>                                                                                      \
  struct SceneNodeCapabilityToKind<TypeName> {                                                     \
    static constexpr SceneNodeCapability::Kind value = SceneNodeCapability::TypeName;              \
  };

} // namespace game

} // namespace sequoia

#endif
