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

#ifndef SEQUOIA_RENDER_NULL_NULLRENDERSYSTEM_H
#define SEQUOIA_RENDER_NULL_NULLRENDERSYSTEM_H

#include "sequoia/Render/Null/NullFwd.h"
#include "sequoia/Render/RenderSystem.h"

namespace sequoia {

namespace render {

/// @brief Null render-system
/// @ingroup null
class SEQUOIA_API NullRenderSystem final : public RenderSystem {

  /// Main-window of the render system
  std::unique_ptr<NullRenderWindow> mainWindow_;

  /// Input system
  std::unique_ptr<NullInputSystem> inputSystem_;

  /// Default shaders
  std::shared_ptr<Shader> defaultVertexShader_, defaultFragmentShader_;

  /// Default program
  std::shared_ptr<Program> defaultProgram_;

public:
  NullRenderSystem(Options* options);

  /// @copydoc RenderSystem::NullRenderSystem
  virtual ~NullRenderSystem();

  /// @copydoc RenderSystem::createWindow
  virtual RenderWindow* createMainWindow(const RenderWindow::WindowHint& hints) override;

  /// @copydoc RenderSystem::destroyMainWindow
  virtual void destroyMainWindow() noexcept override;

  /// @copydoc RenderSystem::getMainWindow
  virtual RenderWindow* getMainWindow() const override;

  /// @copydoc RenderSystem::pollEvents
  virtual void pollEvents() override;

  /// @copydoc RenderSystem::renderOneFrame
  virtual void renderOneFrame(RenderTarget* target) override;

  /// @copydoc RenderSystem::createVertexArrayObject
  virtual std::shared_ptr<Shader> createShader(Shader::ShaderType type,
                                               const std::shared_ptr<File>& path) override;

  /// @copydoc RenderSystem::createProgram
  virtual std::shared_ptr<Program>
  createProgram(const std::set<std::shared_ptr<Shader>>& shaders) override;

  /// @copydoc RenderSystem::createTexture
  virtual std::shared_ptr<Texture>
  createTexture(const std::shared_ptr<Image>& image,
                const TextureParameter& param = TextureParameter()) override;

  /// @copydoc RenderSystem::createIndexBuffer
  virtual std::shared_ptr<VertexData> createVertexData(const VertexDataParameter& param) override {
    return nullptr;
  }

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

  SEQUOIA_NULL_OBJECT(RenderSystem)
};

} // namespace render

} // namespace sequoia

#endif
