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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERSTATE_H
#define SEQUOIA_ENGINE_RENDER_RENDERSTATE_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include <cstdint>
#include <string>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief State of the render pipline of a specific DrawCommand
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
/// To add a new state:
///   1. Add the new state to `RenderState.inc`
///   2. Implement the `Changed` methods in all RenderStateChanges classes
///
/// @ingroup render
struct SEQUOIA_API RenderState {
  RenderState();
  RenderState(const RenderState&) = default;
  RenderState(RenderState&&) = default;

  RenderState& operator=(const RenderState&) = default;
  RenderState& operator=(RenderState&&) = default;

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

#define RENDER_STATE(Type, Name, BitfieldWidth, DefaultValue) Type Name BitfieldWidth;
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  /// Program used in the render-pipeline
  render::Program* Program;

  /// VertexArrayObject used to retrieve the vertex and index buffer objects
  render::VertexData* VertexData;

  /// Textures bound to the specific texture units (map of texture-unit to texture)
  std::unordered_map<int, Texture*> TextureMap;

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Keep track of changes in the RenderState
///
/// To update a complete render state, call `setRenderState` with the new `RenderState`. If a single
/// state **name** changes, the method **nameChanged** will be invoked withe the new state as an
/// argument. Note that the internal state will be updated automatically afterwards.
///
/// @ingroup render
class SEQUOIA_API RenderStateCache {
protected:
  /// Current render-state
  RenderState state_;

protected:
#define RENDER_STATE(Type, Name, BitfieldWidth, DefaultValue)                                      \
  virtual bool Name##Changed(Type value) = 0;
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  /// @brief GPU program changed
  /// @returns `true` if the new program was successfully updated, `false` otherwise
  virtual bool ProgramChanged(Program* program) = 0;

  /// @brief VertexData changed
  /// @returns `true` if the new VertexData was successfully updated, `false` otherwise
  virtual bool VertexDataChanged(VertexData* data, bool bindForDrawing) = 0;

  /// @brief Texture of `textureUnit` was enabled or disabled
  /// @returns `true` if the new texture was successfully updated, `false` otherwise
  virtual bool TextureChanged(int textureUnit, Texture* texture, bool enable) = 0;

public:
  virtual ~RenderStateCache();

  /// @brief Initialize the the state by calling all **Changed** methods
  void initState() noexcept;

  /// @brief Set the interal render-state to `state` and call the appropriate methods for every
  /// state change
  ///
  /// @param state            New RenderState
  /// @returns `true` if the RenderState was successfully updated, `false` otherwise
  bool setRenderState(const RenderState& state) noexcept;

  /// @brief Get the currently active RenderState
  const RenderState& getRenderState() const;
  RenderState& getRenderState();

  /// @brief Convert to string
  std::string toString() const;
};

} // namespace render

} // namespace sequoia

#endif