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
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Keep track and apply changes in the RenderPipeline
///
/// To update a complete render pipeline, call `prepareRenderPipeline` with the new `pipeline`.
/// If a single state `<name>` changes, the method `<name>Changed` will be invoked withe the new
/// state as an argument. Note that the internal state will be updated automatically afterwards.
///
/// @ingroup render
class SEQUOIA_API RenderStateManager {
protected:
#define RENDER_STATE(Type, Name, DefaultValue) virtual bool Name##Changed(const Type& value) = 0;
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  /// @brief GPU program changed
  /// @returns `true` if the new program was successfully updated, `false` otherwise
  virtual bool ProgramChanged(Program* program) = 0;

  /// @brief VertexData changed
  /// @returns `true` if the new VertexData was successfully updated, `false` otherwise
  virtual bool VertexDataChanged(VertexData* data, bool bindForDrawing) = 0;

  /// @brief Texture of `textureUnit` changed
  /// @returns `true` if the new texture was successfully updated, `false` otherwise
  virtual bool TextureChanged(int textureUnit, Texture* texture, bool enable) = 0;

  /// @brief The uniform variable `name` of `program` changed
  virtual void UniformVariableChanged(Program* program, const std::string& name,
                                      const UniformVariable& value) = 0;

public:
  virtual ~RenderStateCache() {}
  RenderStateCache();

  /// @brief Set the interal RenderPipeline to `pipeline` and call the appropriate methods for every
  /// state change
  ///
  /// @param pipeline   New RenderPipeline
  /// @returns `true` if the RenderPipeline was successfully updated, `false` otherwise
  bool setRenderPipeline(const RenderPipeline& pipeline);

  /// @brief Set the uniform variable `name` of `program` to `value`
  bool setUniformVariable(Program* program, const std::string& name, const UniformVariable& value);

  /// @brief Set the given uniform `variables` of `program`
  bool setUniformVariables(Program* program,
                           const std::unordered_map<std::string, UniformVariable>& variables);

  /// @brief Bind the texture-unit/texture pairs
  bool setTextures(const std::unordered_map<int, Texture*>& textures);

  /// @brief Bind the vertex-data
  bool setVertexData(VertexData* vertexData, bool bindForDrawing = true);

  /// @brief Reset the internal state
  ///
  /// This will essentially force a call to all `<name>Changed` methods on the next `set<name>`
  /// invocation.
  void reset();

  /// @brief Invalidate the uniform variable cache of `program`
  void resetUniforms(Program* program);

  /// @brief Convert to string
  std::string toString() const;

protected:
  /// If `reset()` was called, all `<name>Changed` methods of the RenderPipeline need to be executed
  /// (regardless of the state)
  bool forceRenderPipelineUpdate_;

  /// Current pipeline
  RenderPipeline pipeline_;

  /// Current vertex-data
  VertexData* vertexData_;

  /// Current bound textures
  std::unordered_map<int, Texture*> textures_;

  /// Keep track of the values of the uniform variables of the Programs
  std::unordered_map<Program*, std::unordered_map<std::string, UniformVariable>> uniforms_;
};

} // namespace render

} // namespace sequoia

#endif
