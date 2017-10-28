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

#ifndef SEQUOIA_RENDER_DRAWCOMMAND_H
#define SEQUOIA_RENDER_DRAWCOMMAND_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Math/Math.h"
#include "sequoia-engine/Render/RenderState.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <string>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Instructions on how to render an object
///
/// This class should not be directly used but rather constructed via
/// @ref sequoia::game::Drawable::prepareDrawCommand "Drawable::prepareDrawCommand", otherwise
/// certain objects might be in an invalid state (such as the VertexBuffer).
///
/// @ingroup render
class SEQUOIA_API DrawCommand {
public:
  /// @brief Initialize empty DrawCommand
  DrawCommand(const RenderState& state = RenderState())
      : state_(state), modelMatrix_(math::mat4(1.0f)) {}

  DrawCommand(const DrawCommand&) = default;
  DrawCommand(DrawCommand&&) = default;
  DrawCommand& operator=(const DrawCommand&) = default;
  DrawCommand& operator=(DrawCommand&&) = default;

  /// @brief Get/Set the program to use
  Program* getProgram() const noexcept { return state_.Program; }
  void setProgram(Program* program) noexcept { state_.Program = program; }

  /// @brief Get/Set the vertex data to draw
  VertexData* getVertexData() const noexcept { return state_.VertexData; }
  void setVertexData(VertexData* data) noexcept { state_.VertexData = data; }

  /// @brief Get the state of the render pipeline
  const RenderState& getRenderState() const noexcept { return state_; }
  RenderState& getRenderState() noexcept { return state_; }

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
  void setTexture(int textureUnit, Texture* texture) noexcept {
    state_.TextureMap[textureUnit] = texture;
  }

  /// @brief Get the uniform variable by `name`
  UniformVariable& getUniformVariable(const std::string& name) noexcept {
    return variables_.find(name)->second;
  }
  const UniformVariable& getUniformVariable(const std::string& name) const noexcept {
    return variables_.find(name)->second;
  }

  /// @brief Get the uniform variables
  std::unordered_map<std::string, UniformVariable>& getUniformVariables() noexcept {
    return variables_;
  }
  const std::unordered_map<std::string, UniformVariable>& getUniformVariables() const noexcept {
    return variables_;
  }

  /// @brief Set the uniform variable `name` to `value`
  ///
  /// Note that if the uniform variable has already been set, the existing value is overriden.
  void setUniformVariable(const std::string& name, const UniformVariable& value) noexcept {
    variables_[name] = value;
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
    value.toUniformVariableMap(name, variables_, index);
  }

  /// @brief Convert draw command to string
  std::string toString() const;

private:
  /// State of the render state-machine
  RenderState state_;

  /// ModelMatrix used to compute the Model-View-Projection matrix
  math::mat4 modelMatrix_;

  /// Map of the uniform variables send to the associated program. Note that this should only
  /// contain uniforms *specific* to this DrawCommand and not global uniform varaibles of the Scene
  /// (like light positions etc.).
  std::unordered_map<std::string, UniformVariable> variables_;
};

} // namespace render

} // namespace sequoia

#endif
