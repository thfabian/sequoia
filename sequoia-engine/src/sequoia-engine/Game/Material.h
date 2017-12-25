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

#ifndef SEQUOIA_ENGINE_GAME_MATERIAL_H
#define SEQUOIA_ENGINE_GAME_MATERIAL_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Hash.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/Texture.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace sequoia {

namespace game {

/// @brief Material description of a Shape
/// @ingroup game
class SEQUOIA_API Material : public NonCopyable {
public:
  /// @brief Create the material
  /// @note This should never be called manually, use `ShapeManager::load` instead.
  /// @{
  Material();
  Material(std::unordered_map<int, std::shared_ptr<render::Texture>> textures,
           std::unordered_map<std::string, render::UniformVariable> uniforms);
  /// @}

  /// @brief Bind `texture` to `textureUnit`
  ///
  /// This binds the sampler, associated with `textureUnit`, to the specified `texture`. Note that
  /// if a texture has already been bound to the specific `textureUnit`, the texture is replaced
  /// with the newly provided one.
  ///
  /// @param textureUnit    ID of the texture unit starting from 0.
  /// @param texture        Texture to bind
  void setTexture(int textureUnit, const std::shared_ptr<render::Texture>& texture) noexcept {
    textures_[textureUnit] = texture;
  }

  /// @brief Get the texture map
  const std::unordered_map<int, std::shared_ptr<render::Texture>>& getTextures() const noexcept {
    return textures_;
  }

  /// @brief Set the uniform variable `name` to `value`
  ///
  /// Note that if the uniform variable has already been set, the existing value is overriden.
  void setUniformVariable(const std::string& name, const render::UniformVariable& value) noexcept {
    uniforms_[name] = value;
  }

  /// @brief Set the uniform struct `name` to `value`
  ///
  /// @param name     Name of the uniform variable
  /// @param value    Struct to upload to the GPU
  /// @param index    If the struct is part of an array, the index of this struct or -1 to indicate
  ///                 this is a scalar struct
  ///
  /// This calls `StructType::toUniformVariableMap` to retrive all the uniform variables of the
  /// struct. The synopsis of `StructType::toUniformVariableMap` is:
  ///
  /// @code{.cpp}
  /// void toUniformVariableMap(
  ///   const std::string& name,
  ///   std::unordered_map<std::string, UniformVariable>& map,
  ///   int index) const;
  /// @endcode
  ///
  /// In general, the struct should be defined via the `SEQUOIA_UNIFORM_STRUCT` macro which
  /// generates the appropriate member function.
  template <class StructType>
  void setUniformStruct(const std::string& name, const StructType& value, int index = -1) {
    value.toUniformVariableMap(name, uniforms_, index);
  }

  /// @brief Get the uniform variable map
  const std::unordered_map<std::string, render::UniformVariable>& getUniforms() const noexcept {
    return uniforms_;
  }

  /// @brief Convert to string
  std::string toString() const;

private:
  /// Texture-unit/texture pair
  std::unordered_map<int, std::shared_ptr<render::Texture>> textures_;

  /// Uniform variables
  std::unordered_map<std::string, render::UniformVariable> uniforms_;
};

} // namespace game

} // namespace sequoia

#endif
