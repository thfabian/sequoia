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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERER_H
#define SEQUOIA_ENGINE_RENDER_RENDERER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/RenderPipeline.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <cstdint>
#include <string>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Render `DrawCommand`s and keep track/apply changes to the RenderPipeline
/// @ingroup render
class SEQUOIA_API Renderer {
protected:
#define RENDER_STATE(Type, Name, DefaultValue) virtual bool Name##Changed(Type value) = 0;
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
  /// @returns `true` if the new UniformVariable was successfully updated, `false` otherwise
  virtual bool UniformVariableChanged(Program* program, const std::string& name,
                                      const UniformVariable& value) = 0;

  /// @brief The Viewport changed
  /// @returns `true` if the new Viewport was successfully updated, `false` otherwise
  virtual bool ViewportChanged(int x, int y, int width, int height) = 0;

  /// @brief Clear the color buffer
  virtual bool clearColorBuffer() = 0;

  /// @brief Clear the depth buffer
  virtual bool clearDepthBuffer() = 0;

  /// @brief Clear the stencil buffer
  virtual bool clearStencilBuffer() = 0;

  /// @brief Draw the command
  /// @returns `true` if the new DrawCommand was successfully drawn, `false` otherwise
  virtual bool draw(const DrawCommand* drawCommand) = 0;

  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const;

public:
  virtual ~Renderer() {}
  Renderer();

  /// @brief Set the interal RenderPipeline to `pipeline` and call the appropriate methods for every
  /// state change
  ///
  /// @param pipeline   New RenderPipeline
  /// @returns `true` if the RenderPipeline was successfully updated, `false` otherwise
  bool setRenderPipeline(const RenderPipeline& pipeline);

  /// @brief Set the uniform variable `name` of `program` to `value`
  bool setUniform(Program* program, const std::string& name, const UniformVariable& value);

  /// @brief Set texture-unit/texture pairs
  ///
  /// Note that each call potentially disables all texture units which are not present in
  /// `textures`. Thus, you should only call this function once per draw-call.
  bool setTextures(const std::unordered_map<int, Texture*>& textures);

  /// @brief Bind the vertex-data
  bool setVertexData(VertexData* vertexData, bool bindForDrawing = true);

  /// @brief Set the viewport
  bool setViewport(const Viewport* viewport);

  /// @brief Render `command`
  void render(const RenderCommand& command);

  /// @brief Reset the internal state
  ///
  /// This will essentially force a call to all `<name>Changed` methods on the next `set<name>`
  /// invocation.
  virtual void reset();

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

  /// Current viewport
  int x_, y_, width_, height_;

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
