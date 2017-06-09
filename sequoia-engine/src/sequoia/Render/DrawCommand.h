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

#ifndef SEQUOIA_RENDER_DRAWCOMMAND_H
#define SEQUOIA_RENDER_DRAWCOMMAND_H

#include "sequoia/Math/Math.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderState.h"
#include <string>

namespace sequoia {

namespace render {

/// @brief Instructions on how to render the VertexArrayObject
/// @ingroup render
class SEQUOIA_RENDER_API DrawCommand {
public:
  DrawCommand(const RenderState& state = RenderState(),
              const math::mat4& modelMat = math::mat4(1.0f))
      : state_(state), modelMatrix_(modelMat) {}

  /// @brief Copy constructor
  DrawCommand(const DrawCommand&) = default;

  /// @brief Get/Set the program to use
  Program* getProgram() const noexcept { return state_.Program; }
  void setProgram(Program* program) noexcept { state_.Program = program; }

  /// @brief Get/Set the vertex data to draw
  VertexArrayObject* getVertexArrayObject() const noexcept { return state_.VertexArrayObject; }
  void setVertexArrayObject(VertexArrayObject* vao) noexcept { state_.VertexArrayObject = vao; }

  /// @brief Get the state of the render pipeline
  const RenderState& getRenderState() const noexcept { return state_; }
  RenderState& getRenderState() noexcept { return state_; }

  /// @brief Get/Set the model matrix
  const math::mat4& getModelMatrix() const noexcept { return modelMatrix_; }
  void setModelMatrix(const math::mat4& modelMatrix) noexcept { modelMatrix_ = modelMatrix; }

  /// @brief Convert draw command to string
  std::string toString() const;

private:
  RenderState state_;
  math::mat4 modelMatrix_;
};

} // namespace render

} // namespace sequoia

#endif
