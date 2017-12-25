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

#ifndef SEQUOIA_ENGINE_RENDER_DRAWCOMMAND_H
#define SEQUOIA_ENGINE_RENDER_DRAWCOMMAND_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Math/Math.h"
#include "sequoia-engine/Render/Texture.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include "sequoia-engine/Render/VertexData.h"
#include <string>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Encapsulate all information required by a draw command to render a `game::Shape`
///
/// Note that the DrawCommand of Shape can be extracted via `game::Drawable::makeDrawCommand()`.
///
/// @ingroup render
class SEQUOIA_API DrawCommand {
public:
  DrawCommand() = default;
  DrawCommand(VertexData* data, const math::mat4& modelMatrix)
      : data_(data), modelMatrix_(modelMatrix) {}

  /// @brief Get/Set the vertex data
  VertexData* getVertexData() const noexcept { return data_; }
  void setVertexData(VertexData* data) noexcept { data_ = data; }

  /// @brief Get/Set the model matrix
  const math::mat4& getModelMatrix() const noexcept { return modelMatrix_; }
  void setModelMatrix(const math::mat4& modelMatrix) noexcept { modelMatrix_ = modelMatrix; }

  /// @brief Bind `texture` to `textureUnit`
  ///
  /// This binds the sampler, associated with `textureUnit`, to the specified `texture`. Note that
  /// if a texture has already been bound to the specific `textureUnit`, the texture is replaced
  /// with the newly provided one.
  ///
  /// @param textureUnit    ID of the texture unit starting from 0.
  /// @param texture        Texture to bind
  void setTexture(int textureUnit, Texture* texture) noexcept { textures_[textureUnit] = texture; }

  /// @brief Get the texture map
  const std::unordered_map<int, Texture*>& getTextures() const noexcept { return textures_; }

  /// @brief Set the uniform variable `name` to `value`
  ///
  /// Note that if the uniform variable has already been set, the existing value is overriden.
  void setUniformVariable(const std::string& name, const UniformVariable& value) noexcept {
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
  const std::unordered_map<std::string, UniformVariable>& getUniforms() const noexcept {
    return uniforms_;
  }

  /// @brief Convert draw command to string
  std::string toString() const;

private:
  /// Vertex data of the mesh
  VertexData* data_ = nullptr;

  /// Matrix used to construct the world transformation of vertex data
  math::mat4 modelMatrix_ = math::mat4(1.0f);

  /// Texture data of the material (associate texture units to actual textures)
  std::unordered_map<int, Texture*> textures_;

  /// Uniform variables which are *specific* to this DrawCommand (mostly material properties)
  std::unordered_map<std::string, UniformVariable> uniforms_;
};

} // namespace render

} // namespace sequoia

#endif
