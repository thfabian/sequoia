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

  /// @brief Draw mode
  enum DrawModeKind {
    DM_Triangles = 0 ///< Treats each triplet of vertices as an independent triangle
  };

  /// Enable Z-Buffer
  bool DepthTest = true;

  /// Function to fill the Z-Buffer
  DepthFuncKind DepthFunc = DF_Less;

  /// Mode of drawing
  DrawModeKind DrawMode = DM_Triangles;

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Instructions on how to render the VertexArrayObject
/// @ingroup render
class SEQUOIA_RENDER_API DrawCommand {
public:
  DrawCommand(Program* program, VertexArrayObject* vao, RenderState* state,
              const math::mat4& modelMat)
      : program_(program), vao_(vao), state_(state), modelMat_(modelMat) {}

  /// @brief Get the program to use
  Program* getProgram() const noexcept { return program_; }

  /// @brief Get the vertex data to draw
  VertexArrayObject* getVertexArrayObject() const noexcept { return vao_; }

  /// @brief Get the state of the render pipeline
  RenderState* getRenderState() const noexcept { return state_; }

  /// @brief Get the model matrix
  const math::mat4& getModelMatrix() const noexcept { return modelMat_; }

  /// @brief Convert draw command to string
  std::string toString() const;

private:
  Program* program_;
  VertexArrayObject* vao_;
  RenderState* state_;
  math::mat4 modelMat_;
};

} // namespace render

} // namespace sequoia

#endif
