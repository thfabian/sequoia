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

#include "sequoia/Render/RenderSystem.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace render {

class GLRenderer;
class GLInputSystem;

/// @brief OpenGL render-system
/// @ingroup gl
class SEQUOIA_RENDER_API GLRenderSystem : public RenderSystem {
  std::vector<std::shared_ptr<RenderTarget>> renderTargets_;

  std::unordered_map<RenderTarget*, GLRenderer*> rendererMap_;
  std::unordered_map<RenderTarget*, GLInputSystem*> inputSystemMap_;

public:
  /// @brief Initialize GLFW
  /// @throws RenderSystemInitException    Initialization of GLFW failed
  GLRenderSystem();

  /// @brief Terminates GLFW
  virtual ~GLRenderSystem();

  /// @copydoc RenderSystem::createWindow
  virtual RenderWindow* createWindow(const RenderWindow::WindowHint& hints) override;

  /// @copydoc RenderSystem::destroyTarget
  virtual void destroyTarget(RenderTarget* target) override;

  /// @copydoc RenderSystem::pollEvents
  virtual void pollEvents() override;

  /// @copydoc RenderSystem::renderOneFrame
  virtual void renderOneFrame() override;

  /// @copydoc RenderSystem::swapBuffers
  virtual void swapBuffers() override;

  /// @brief Create a new VBO
  virtual std::unique_ptr<VertexArrayObject> createVertexArrayObject() override;

  /// @brief Load the shader from source if it has not already been loaded
  virtual std::shared_ptr<Shader> loadShader(RenderTarget* target, Shader::ShaderType type,
                                             const platform::String& path) override;

  /// @brief Link the shaders into a program if a program of the given shaders does not yet exist
  virtual std::shared_ptr<Program>
  createProgram(RenderTarget* target, const std::set<std::shared_ptr<Shader>>& shaders) override;

  /// @brief Add the keyboard `listener` to `target`
  virtual void addKeyboardListener(RenderTarget* target, KeyboardListener* listener) override;

  /// @brief Remove the keyboard `listener` of `target`
  virtual void removeKeyboardListener(RenderTarget* target, KeyboardListener* listener) override;

  /// @brief Add the mouse `listener` to `target`
  virtual void addMouseListener(RenderTarget* target, MouseListener* listener) override;

  /// @brief Remove the mouse `listener` of `target`
  virtual void removeMouseListener(RenderTarget* target, MouseListener* listener) override;

  /// @brief Register the InputSystem of the `target`
  void registerInputSystem(RenderTarget* target, GLInputSystem* inputSystem);

  /// @brief Get the renderer of `target`
  GLInputSystem* getInputSystem(RenderTarget* target) const noexcept;

  /// @brief Register the Renderer of the `target`
  void registerRenderer(RenderTarget* target, GLRenderer* renderer);

  /// @brief Get the renderer of `target`
  GLRenderer* getRenderer(RenderTarget* target) const noexcept;

  SEQUOIA_GL_OBJECT(RenderSystem);
};

} // namespace render

} // namespace sequoia

#endif
