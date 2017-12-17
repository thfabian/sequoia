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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERSTATEMANAGER_H
#define SEQUOIA_ENGINE_RENDER_RENDERSTATEMANAGER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include <cstdint>
#include <string>

namespace sequoia {

namespace render {

/// @brief Keep track of changes in the RenderPipeline
///
/// To update a complete render pipeline, call `setRenderPipeline` with the new `RenderPipeline`. 
/// If a single state **name** changes, the method **nameChanged** will be invoked withe the new 
/// state as an argument. Note that the internal state will be updated automatically afterwards.
///
/// @ingroup render
class SEQUOIA_API RenderStateManager {
protected:
  /// Current pipeline
  RenderPipeline pipeline_;

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
