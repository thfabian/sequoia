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

#ifndef SEQUOIA_GAME_POINTLIGHT_H
#define SEQUOIA_GAME_POINTLIGHT_H

#include "sequoia/Core/Color.h"
#include "sequoia/Game/Emittable.h"

namespace sequoia {

namespace game {

/// @brief Point-light emitter
/// @ingroup game
class SEQUOIA_API PointLight final : public Emittable {
public:
  /// @brief Virtual destructor
  virtual ~PointLight();

  /// @brief Construct the emitter
  PointLight(SceneNode* node, math::vec3 power, Color color = Color::White);

  /// @copydoc Emittable::toUniformVariableMap
  virtual void toUniformVariableMap(std::unordered_map<std::string, UniformVariable>& map,
                                    int index) override;

  /// @copydoc Emittable::getUniformVariableName
  virtual const char* getUniformVariableName() const override;

  /// @copydoc SceneNodeCapability::update
  virtual void update(const SceneNodeUpdateEvent& event) override;

  /// @copydoc SceneNodeCapability::clone
  virtual std::shared_ptr<SceneNodeCapability> clone(SceneNode* node) const override;

  /// @brief Get/Set the power of the light
  /// @{
  math::vec3 getPower() const { return power_; }
  void setPower(const math::vec3& power) { power_ = power; }
  /// @}

  /// @brief Get/Set the color of the light
  /// @{
  Color getColor() const { return color_; }
  void setColor(const Color& color) { color_ = color; }
  /// @}

  static bool classof(const Emittable* emittable) noexcept {
    return emittable->getKind() == EK_PointLight;
  }

protected:
  /// @copydoc SceneNodeCapability::toStringImpl
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  using Base = Emittable;

  /// Power of the light
  math::vec3 power_;

  /// Color of the light
  Color color_;
};

} // namespace game

} // namespace sequoia

#endif
