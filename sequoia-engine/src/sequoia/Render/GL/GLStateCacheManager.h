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

#ifndef SEQUOIA_RENDER_GL_GLSTATECACHEMANAGER_H
#define SEQUOIA_RENDER_GL_GLSTATECACHEMANAGER_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/FrameListener.h"
#include "sequoia/Render/RenderFwd.h"
#include <memory>
#include <unordered_map>

namespace sequoia {

namespace render {

class GLRenderStateCache;

/// @brief Manager of the OpenGL state-machine
///
/// This keeps an in-memory copy of the OpenGL state-machine to avoid unnecessary state changes.
///
/// @ingroup gl
class SEQUOIA_API GLStateCacheManager : public FrameListener, public NonCopyable {

  /// In-memory version of the OpenGL state
  std::shared_ptr<GLRenderStateCache> stateCache_;

public:
  GLStateCacheManager();

  /// @brief Draw the `command` and update the RenderState *if* necessary
  void draw(DrawCommand* command);

  /// @brief Get the currently active RenderState
  const RenderState& getRenderState() const;

  /// @brief Bind the `program`
  void bindProgram(Program* program);

  /// @brief Unbind any programs
  void unbindProgram();

  /// @brief Bind the given vertex array object `vao`
  void bindVertexArrayObject(VertexArrayObject* vao);

  /// @brief Unbind any vertex array objects
  void unbindVertexArrayObject();

  /// @brief Bind the given frame buffer object `fbo`
  void bindFrameBufferObject(FrameBufferObject* fbo);

  /// @brief Unbind any frame buffer objects
  void unbindFrameBufferObject();

  /// @brief Bind the `texture` to `textureUnit`
  void bindTexture(int textureUnit, Texture* texture);

  /// @brief Unbind any texture from `textureUnit`
  void unbindTexture(int textureUnit);

  void frameListenerRenderingBegin(std::size_t frameID);
  void frameListenerRenderingEnd(std::size_t frameID);
};

} // namespace render

} // namespace sequoia

#endif
