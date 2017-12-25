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

#ifndef SEQUOIA_ENGINE_RENDER_GL_GLRENDERSYSTEM_H
#define SEQUOIA_ENGINE_RENDER_GL_GLRENDERSYSTEM_H

#include "sequoia-engine/Render/RenderSystem.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace render {

class GLRenderer;
class GLRenderWindow;
class GLStateCacheManager;
class NativeGLContext;

/// @brief OpenGL render-system
/// @ingroup gl
class SEQUOIA_API GLRenderSystem final : public RenderSystem {

  /// Main OpenGL context
  std::shared_ptr<NativeGLContext> mainContext_;

  /// Main-window of the render system
  std::unique_ptr<GLRenderWindow> mainWindow_;

  /// OpenGL renderer
  std::unique_ptr<GLRenderer> renderer_;

public:
  GLRenderSystem(const std::shared_ptr<Options>& options);

  /// @brief Terminates GLFW
  virtual ~GLRenderSystem();

  /// @copydoc RenderSystem::createWindow
  virtual RenderWindow* createMainWindow(const RenderWindow::WindowHint& hints) override;

  /// @copydoc RenderSystem::destroyMainWindow
  virtual void destroyMainWindow() noexcept override;

  /// @copydoc RenderSystem::getMainWindow
  virtual RenderWindow* getMainWindow() const override;

  /// @brief Get the main-window
  GLRenderWindow* getMainGLWindow() const;

  /// @copydoc RenderSystem::pollEvents
  virtual void pollEvents() override;

  /// @brief Load the shader from source if it has not already been loaded
  virtual std::shared_ptr<Shader> createShader(Shader::ShaderType type, const std::string& filename,
                                               const std::string& source) override;

  /// @brief Link the shaders into a program if a program of the given shaders does not yet exist
  virtual std::shared_ptr<Program>
  createProgram(const std::set<std::shared_ptr<Shader>>& shaders) override;

  /// @copydoc RenderSystem::createTexture
  virtual std::shared_ptr<Texture>
  createTexture(const std::shared_ptr<Image>& image,
                const TextureParameter& param = TextureParameter()) override;

  /// @copydoc RenderSystem::createVertexData
  virtual std::shared_ptr<VertexData> createVertexData(const VertexDataParameter& param) override;

  /// @copydoc RenderSystem::addKeyboardListener
  virtual void addKeyboardListener(KeyboardListener* listener) override;

  /// @copydoc RenderSystem::removeKeyboardListener
  virtual void removeKeyboardListener(KeyboardListener* listener) override;

  /// @copydoc RenderSystem::addMouseListener
  virtual void addMouseListener(MouseListener* listener) override;

  /// @copydoc RenderSystem::removeMouseListener
  virtual void removeMouseListener(MouseListener* listener) override;

  /// @brief Get the OpenGL renderer
  virtual Renderer* getRenderer() const override;
  GLRenderer* getGLRenderer() const;

  SEQUOIA_GL_OBJECT(RenderSystem)
};

/// @brief Get a reference to the currently active `GLRenderSystem`
/// @ingroup gl
/// @{
SEQUOIA_API extern GLRenderSystem& getGLRenderSystem() noexcept;
SEQUOIA_API extern GLRenderSystem* getGLRenderSystemPtr() noexcept;
/// @}

} // namespace render

} // namespace sequoia

#endif
