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

#ifndef SEQUOIA_RENDER_RENDERSYSTEM_H
#define SEQUOIA_RENDER_RENDERSYSTEM_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Image.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Core/Singleton.h"
#include "sequoia/Render/Input.h"
#include "sequoia/Render/RenderFwd.h"
#include "sequoia/Render/RenderSystemObject.h"
#include "sequoia/Render/RenderWindow.h"
#include "sequoia/Render/Shader.h"
#include "sequoia/Render/Texture.h"
#include "sequoia/Render/VertexArrayObject.h"
#include <memory>
#include <set>
#include <string>

namespace sequoia {

namespace render {

/// @brief Currently active render-system
///
/// The render-system takes care of window and context creation as well as setting up the IO
/// devices and rendering the scene. Note that there can only be one RenderSystem active at the
/// time.
///
/// @ingroup render
class SEQUOIA_API RenderSystem : public Singleton<RenderSystem>,
                                 public Listenable<InputEventListener>,
                                 public RenderSystemObject {
public:
  /// @brief Create the RenderSystem of the given `kind`
  /// @remark Terminates the program on failure
  static std::unique_ptr<RenderSystem> create(RenderSystemKind kind);

  /// @brief Terminate the render-system
  virtual ~RenderSystem();

  /// @brief Create the main-window (if a main-window is already active, the old one will be
  /// destroyed first)
  /// @returns the created window
  virtual RenderWindow* createMainWindow(const RenderWindow::WindowHint& hints) = 0;

  /// @brief Destroys the currently active main-window
  virtual void destroyMainWindow() noexcept = 0;

  /// @brief Get the main-window
  virtual RenderWindow* getMainWindow() const = 0;

  /// @brief Processes events that are in the event queue
  virtual void pollEvents() = 0;

  /// @brief Render one frame into `target`
  virtual void renderOneFrame(RenderTarget* target) = 0;

  /// @brief Create a new VertexArrayObject for `target`
  virtual std::unique_ptr<VertexArrayObject> createVertexArrayObject() = 0;

  /// @brief Create a shader from source for `target`
  virtual std::shared_ptr<Shader> createShader(Shader::ShaderType type,
                                               const std::shared_ptr<File>& file) = 0;

  /// @brief Create a GPU program from the given `shaders` for `target`
  virtual std::shared_ptr<Program>
  createProgram(const std::set<std::shared_ptr<Shader>>& shaders) = 0;

  /// @brief Create a texture of `image` (using texture parameters `param`)
  virtual std::shared_ptr<Texture>
  createTexture(const std::shared_ptr<Image>& image,
                const TextureParameter& param = TextureParameter()) = 0;

  /// @brief Add the keyboard `listener`
  virtual void addKeyboardListener(KeyboardListener* listener) = 0;

  /// @brief Remove the keyboard `listener`
  virtual void removeKeyboardListener(KeyboardListener* listener) = 0;

  /// @brief Add the mouse `listener`
  virtual void addMouseListener(MouseListener* listener) = 0;

  /// @brief Remove the mouse `listener`
  virtual void removeMouseListener(MouseListener* listener) = 0;

  /// @brief Load the default vertex and fragment shaders and link them into a program
  ///
  /// @param defaultVertexShaderFile    File containing the default vertex shader
  /// @param defaultFragmentShaderFile  File containing the default fragment shader
  virtual void loadDefaultShaders(const std::shared_ptr<File>& defaultVertexShaderFile,
                                  const std::shared_ptr<File>& defaultFragmentShaderFile) = 0;

  /// @brief Get the default vertex shader
  virtual const std::shared_ptr<Shader>& getDefaultVertexShader() const = 0;

  /// @brief Get the default fragment shader
  virtual const std::shared_ptr<Shader>& getDefaultFragmentShader() const = 0;

  /// @brief Get the default fragment shader
  virtual const std::shared_ptr<Program>& getDefaultProgram() const = 0;

  /// @brief Set if we run in debug-mode
  void setDebugMode(bool debugMode);

  /// @brief Check if we run in debug-mode
  bool debugMode() const;

protected:
  RenderSystem(RenderSystemKind kind);

private:
  bool debugMode_;
};

} // namespace render

} // namespace sequoia

#endif
