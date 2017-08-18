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

#include "sequoia/Core/Export.h"
#include "sequoia/Math/Math.h"
#include "sequoia/Render/RenderState.h"
#include "sequoia/Render/UniformVariable.h"
#include <string>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Instructions on how to render the VertexArrayObject
///
/// This class should not be directly used but rather constructed via
/// @ref sequoia::game::Drawable::prepareDrawCommand "Drawable::prepareDrawCommand", otherwise the
/// VertexArrayObjects might be in an invalid state.
///
/// @ingroup render
class SEQUOIA_API DrawCommand {
public:
  /// @brief Initializ empty DrawCommand
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
  VertexArrayObject* getVertexArrayObject() const noexcept { return state_.VertexData; }
  void setVertexArrayObject(VertexArrayObject* vao) noexcept { state_.VertexData = vao; }

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
  void setUniformVariable(const std::string& name, const UniformVariable& value) noexcept {
    variables_[name] = value;
  }

  /// @brief Convert draw command to string
  std::string toString() const;

private:
  /// State of the render state-machine
  RenderState state_;

  /// ModelMatrix used to compute the Model-View-Projection matrix
  math::mat4 modelMatrix_;

  /// Map of the uniform variables send to the associated program
  std::unordered_map<std::string, UniformVariable> variables_;

  // TODO: keep a list of "global" uniform variables from the scene and keep those in the
  // drawCommandList --> GlobalRenderState --> sharedVariables_
  // std::vector<int> // only store the id
};

} // namespace render

} // namespace sequoia

#endif
