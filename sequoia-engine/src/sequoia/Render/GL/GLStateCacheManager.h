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

#ifndef SEQUOIA_RENDER_GL_GLSTATECACHEMANAGER_H
#define SEQUOIA_RENDER_GL_GLSTATECACHEMANAGER_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/DrawCommand.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderState.h"
#include <unordered_map>

namespace sequoia {

namespace render {

class GLProgram;
class GLVertexArrayObject;

/// @brief Manager of the OpenGL state-machine
///
/// This keeps an in-memory copy of the OpenGL state-machine to avoid unnecessary state changes.
///
/// @ingroup gl
class SEQUOIA_RENDER_API GLStateCacheManager : public NonCopyable {

  /// Bound GPU program
  GLProgram* program_;

  /// Bound VAO
  GLVertexArrayObject* vao_;

  /// Current RenderState
  std::unique_ptr<RenderStateCache> state_;

public:
  GLStateCacheManager();

  /// @brief Set the `RenderState`
  void setRenderState(const RenderState& renderState);

  /// @brief Set the program to use in the render-pipeline
  void setProgram(GLProgram* program);

  /// @brief Set the VertexArrayObject object to bind
  void draw(DrawCommand* command);

  /// @brief Get the currently active RenderState
  const RenderState& getRenderState() const;
};

} // namespace render

} // namespace sequoia

#endif
