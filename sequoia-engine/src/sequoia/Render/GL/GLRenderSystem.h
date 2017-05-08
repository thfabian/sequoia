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

#ifndef SEQUOIA_RENDER_GL_GLRENDERSYSTEM_H
#define SEQUOIA_RENDER_GL_GLRENDERSYSTEM_H

#include "sequoia/Render/RenderSystemImpl.h"
#include <memory>
#include <vector>

namespace sequoia {

namespace render {

/// @brief OpenGL render-system
/// @ingroup gl
class SEQUOIA_RENDER_API GLRenderSystem : public RenderSystemImpl {
  std::vector<std::shared_ptr<RenderTarget>> renderTargets_;

public:
  /// @brief Initialize GLFW
  /// @throws RenderSystemInitException    Initialization of GLFW failed
  GLRenderSystem();

  /// @brief Terminates GLFW
  virtual ~GLRenderSystem();

  /// @copydoc RenderSystemImpl::createWindow
  virtual RenderWindow* createWindow(const std::string& title) override;

  /// @copydoc RenderSystemImpl::pollEvents
  virtual void pollEvents() override;

  /// @copydoc RenderSystemImpl::renderOneFrame
  virtual void renderOneFrame() override;

  /// @copydoc RenderSystemImpl::swapBuffers
  virtual void swapBuffers() override;
};

} // namespace render

} // namespace sequoia

#endif
