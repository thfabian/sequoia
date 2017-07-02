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
class GLRenderWindow;
class GLStateCacheManager;

/// @brief OpenGL render-system
/// @ingroup gl
class SEQUOIA_API GLRenderSystem final : public RenderSystem {

  /// Main-window of the render system (also holds the OpenGL context)
  std::shared_ptr<GLRenderWindow> mainWindow_;
  
public:
  /// @brief Initialize GLFW
  /// @throws RenderSystemInitException    Initialization of GLFW failed
  GLRenderSystem();

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

  /// @copydoc RenderSystem::renderOneFrame
  virtual void renderOneFrame(RenderTarget* target) override;

  /// @copydoc RenderSystem::createVertexArrayObject
  virtual std::unique_ptr<VertexArrayObject> createVertexArrayObject() override;

  /// @brief Load the shader from source if it has not already been loaded
  virtual std::shared_ptr<Shader> createShader(Shader::ShaderType type,
                                               const std::shared_ptr<File>& path) override;

  /// @brief Link the shaders into a program if a program of the given shaders does not yet exist
  virtual std::shared_ptr<Program>
  createProgram(const std::set<std::shared_ptr<Shader>>& shaders) override;

  /// @copydoc RenderSystem::createTexture
  virtual std::shared_ptr<Texture>
  createTexture(const std::shared_ptr<Image>& image,
                const TextureParameter& param = TextureParameter()) override;

  /// @copydoc RenderSystem::addKeyboardListener
  virtual void addKeyboardListener(KeyboardListener* listener) override;

  /// @copydoc RenderSystem::removeKeyboardListener
  virtual void removeKeyboardListener(KeyboardListener* listener) override;

  /// @copydoc RenderSystem::addMouseListener
  virtual void addMouseListener(MouseListener* listener) override;

  /// @copydoc RenderSystem::removeMouseListener
  virtual void removeMouseListener(MouseListener* listener) override;

  /// @copydoc RenderSystem::loadDefaultShaders
  virtual void loadDefaultShaders(const std::shared_ptr<File>& defaultVertexShaderFile,
                                  const std::shared_ptr<File>& defaultFragmentShaderFile) override;

  /// @copydoc RenderSystem::getDefaultVertexShader
  virtual const std::shared_ptr<Shader>& getDefaultVertexShader() const override;

  /// @copydoc RenderSystem::getDefaultFragmentShader
  virtual const std::shared_ptr<Shader>& getDefaultFragmentShader() const override;

  /// @copydoc RenderSystem::getDefaultProgram
  virtual const std::shared_ptr<Program>& getDefaultProgram() const override;

  /// @brief Get the Renderer of the main-window
  GLRenderer* getRenderer();
  GLRenderer* getRenderer() const;  
  
  /// @brief Get the InputSystem of the main-window
  GLInputSystem* getInputSystem();
  GLInputSystem* getInputSystem() const;  

  /// @brief Get the state cache manager of the main-window
  GLStateCacheManager* getStateCacheManager();
  
  /// @brief Check if `extension` is supported
  bool isExtensionSupported(const std::string& extension) noexcept;

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
