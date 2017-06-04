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
#include <string>

namespace sequoia {

namespace render {

class Program;
class VertexArrayObject;

/// @brief State of the render pipline
/// @ingroup render
struct SEQUOIA_RENDER_API RenderState {
  RenderState() = default;
  RenderState(const RenderState&) = default;

  /// @brief Specifies the function used to compare each incoming pixel depth value with the depth
  /// value present in the depth buffer
  enum DepthFuncKind {
    DF_Never = 0,    ///< Never passes
    DF_Less,         ///< Passes if the incoming depth value is less than the stored depth value
    DF_Equal,        ///< Passes if the incoming depth value is equal to the stored depth value
    DF_LessEqual,    ///< Passes if the incoming depth value is less than or equal to the stored
                     ///  depth value
    DF_Greater,      ///< Passes if the incoming depth value is greater than the stored depth value
    DF_NotEqual,     ///< Passes if the incoming depth value is not equal to the stored depth value
    DF_GreaterEqual, ///< Passes if the incoming depth value is greater than or equal to the
                     ///  stored depth value
    DF_Always        ///< Always passes
  };

  /// Enable Z-Buffer
  bool DepthTest = true;

  /// Function to fill the Z-Buffer
  DepthFuncKind DepthFunc = DF_Less;

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Instructions on how to render the VertexArrayObject
/// @ingroup render
class SEQUOIA_RENDER_API DrawCommand {
public:
  DrawCommand(Program* program, VertexArrayObject* vao,
              const math::mat4& modelMat = math::mat4(1.0f), RenderState state = RenderState())
      : program_(program), vao_(vao), state_(state), modelMatrix_(modelMat) {}

  /// @brief Copy constructor
  DrawCommand(const DrawCommand&) = default;

  /// @brief Get/Set the program to use
  Program* getProgram() const noexcept { return program_; }
  void setProgram(Program* program) noexcept { program_ = program; }

  /// @brief Get/Set the vertex data to draw
  VertexArrayObject* getVertexArrayObject() const noexcept { return vao_; }
  void setVertexArrayObject(VertexArrayObject* vao) noexcept { vao_ = vao; }

  /// @brief Get the state of the render pipeline
  const RenderState& getRenderState() const noexcept { return state_; }
  RenderState& getRenderState() noexcept { return state_; }

  /// @brief Get/Set the model matrix
  const math::mat4& getModelMatrix() const noexcept { return modelMatrix_; }
  void setModelMatrix(const math::mat4& modelMatrix) noexcept { modelMatrix_ = modelMatrix; }

  /// @brief Convert draw command to string
  std::string toString() const;

private:
  Program* program_;
  VertexArrayObject* vao_;
  RenderState state_;
  math::mat4 modelMatrix_;
};

} // namespace render

} // namespace sequoia

#endif
