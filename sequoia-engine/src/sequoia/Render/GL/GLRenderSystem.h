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

#include "sequoia/Core/Singleton.h"
#include "sequoia/Render/RenderSystem.h"
#include <memory>
#include <vector>

namespace sequoia {

namespace render {

/// @brief OpenGL render-system
/// @ingroup gl
class SEQUOIA_RENDER_API GLRenderSystem : public RenderSystem, public Singleton<GLRenderSystem> {

  /// Registered RenderTargets
  std::vector<std::shared_ptr<RenderTarget>> renderTargets_;

public:
  /// @brief Initialize GLFW
  /// @throws RenderSystemInitException    Initialization of GLFW failed
  GLRenderSystem();

  /// @brief Terminates GLFW
  virtual ~GLRenderSystem();

  /// @copydoc RenderSystem::createWindow
  virtual RenderWindow* createWindow(const std::string& title) override;

  /// @copydoc RenderSystem::pollEvents
  virtual void pollEvents() override;

  /// @copydoc RenderSystem::renderOneFrame
  virtual void renderOneFrame() override;

  /// @copydoc RenderSystem::swapBuffers
  virtual void swapBuffers() override;

  /// @brief Load the shader from source if it has not already been loaded
  Shader* loadShader(RenderTarget* target, const platform::Path& path) override;

  /// @brief Link the shaders into a program if a program of the given shaders does not yet exist
  GPUProgram* createProgram(RenderTarget* target, Shader* vertex, Shader* fragment) override;
};

} // namespace render

} // namespace sequoia

#endif
