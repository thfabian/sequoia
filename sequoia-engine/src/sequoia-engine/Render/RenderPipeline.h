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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERPIPELINE_H
#define SEQUOIA_ENGINE_RENDER_RENDERPIPELINE_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include <cstdint>
#include <string>

namespace sequoia {

namespace render {

/// @brief Definition of a render pipeline
///
/// <table>
/// <caption id="multi_row">Available render-states</caption>
/// <tr><th>Type                          <th>Name             <th>Default Value
///     <th>Explanation
/// <tr><td>bool                          <td>DepthTest        <td>true
///     <td>Enable Z-buffer (depth) test.
/// <tr><td>RenderState::DepthFuncKind    <td>DetpthFunc       <td>RenderState::DF_Less
///     <td>Function used for depth testing.
/// </table>
///
/// To add a new render state <name>:
///   1. Add the new state <name> to `RenderState.inc`
///   2. Implement the `<name>Changed` methods in all Renderer (missing implementations will result
///      in compile time errors).
///
/// @ingroup render
struct SEQUOIA_API RenderPipeline {
  /// @brief Specifies the function used to compare each incoming pixel depth value with the depth
  /// value present in the depth buffer
  enum DepthFuncKind : std::uint8_t {
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

#define RENDER_STATE(Type, Name, DefaultValue) Type Name = DefaultValue;
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  /// Program installed in the render-pipeline
  render::Program* Program = nullptr;

  /// @brief Convert to string
  std::string toString() const;
};

} // namespace render

} // namespace sequoia

#endif
