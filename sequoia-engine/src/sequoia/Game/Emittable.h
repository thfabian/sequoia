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

#ifndef SEQUOIA_GAME_EMITTABLE_H
#define SEQUOIA_GAME_EMITTABLE_H

#include "sequoia/Game/GameFwd.h"
#include "sequoia/Game/SceneNodeCapability.h"
#include "sequoia/Render/RenderFwd.h"
#include "sequoia/Render/UniformVariable.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace sequoia {

namespace game {

/// @brief Base class of all emitters which is modeled as a capability of a SceneNode
/// @ingroup game
class SEQUOIA_API Emittable : public SceneNodeCapability {
public:
  enum EmitterKind { EK_PointLight = 0, NumEmitter };

  /// @brief Virtual destructor
  virtual ~Emittable();

  /// @brief Construct the emitter
  Emittable(SceneNode* node, EmitterKind kind);

  /// @brief Does the node currently emit light?
  bool isActive() const { return active_; }

  /// @brief Set if the node emits light
  void setActive(bool active) { active_ = active; }

  /// @brief Get the kind of emitter
  EmitterKind getKind() const { return kind_; }

  /// @brief Convert the emitter to a sequence of uniform variables which are sent to the GPU
  /// Program
  virtual void toUniformVariableMap(std::unordered_map<std::string, UniformVariable>& map,
                                    int index) = 0;

  /// @brief Get the name of the emitter
  ///
  /// This also serves as the uniform variable name for GPU programs (i.e `"e_" + getName()`)
  virtual const char* getName() const = 0;

  /// @copydoc SceneNodeCapability::toString
  virtual std::string toString() const override final;

  /// @copydoc SceneNodeCapability::update
  virtual void update(const SceneNodeUpdateEvent& event) override = 0;

  /// @copydoc SceneNodeCapability::clone
  virtual std::shared_ptr<SceneNodeCapability> clone(SceneNode* node) const override = 0;

protected:
  /// @brief Implementation of `toString` which returns the class name and stringified members
  virtual std::pair<std::string, std::string> toStringImpl() const = 0;

  /// @brief Get the uniform variable name of the emitter
  std::string getUniformVariableName() const;

private:
  using Base = SceneNodeCapability;

  /// Kind of emitter
  EmitterKind kind_;

  /// Is emitting enabled?
  bool active_;
};

SEQUOIA_REGISTER_SCENENODE_CAPABILITY(Emittable)

} // namespace game

} // namespace sequoia

#endif
