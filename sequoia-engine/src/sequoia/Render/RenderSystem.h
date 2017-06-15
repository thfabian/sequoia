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

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Core/Singleton.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/Input.h"
#include "sequoia/Render/RenderFwd.h"
#include "sequoia/Render/RenderSystemObject.h"
#include "sequoia/Render/RenderWindow.h"
#include "sequoia/Render/Shader.h"
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
class SEQUOIA_API RenderSystem : public Singleton<RenderSystem>, public RenderSystemObject {
public:
  /// @brief Create the RenderSystem of the given `kind`
  /// @remark Terminates the program on failure
  static std::unique_ptr<RenderSystem> create(RenderSystemKind kind);

  /// @brief Terminate the render-system
  virtual ~RenderSystem();

  /// @brief Create a new RenderWindow
  /// @returns the created window
  virtual RenderWindow* createWindow(const RenderWindow::WindowHint& hints) = 0;

  /// @brief Manually destroy the RenderTarget
  virtual void destroyTarget(RenderTarget* target) = 0;

  /// @brief Processes events that are in the event queue
  virtual void pollEvents() = 0;

  /// @brief Render one frame into the target of each active registered RenderTarget
  virtual void renderOneFrame() = 0;

  /// @brief Swap the buffers and display the next frame for each active registered RenderTarget
  virtual void swapBuffers() = 0;

  /// @brief Create a new VertexArrayObject for `target`
  virtual std::unique_ptr<VertexArrayObject> createVertexArrayObject(RenderTarget* target) = 0;

  /// @brief Load a shader from source for `target`
  virtual std::shared_ptr<Shader> loadShader(RenderTarget* target, Shader::ShaderType type,
                                             const std::shared_ptr<File>& file) = 0;

  /// @brief Create a GPU program from the given `shaders` for `target`
  virtual std::shared_ptr<Program>
  createProgram(RenderTarget* target, const std::set<std::shared_ptr<Shader>>& shaders) = 0;

  /// @brief Add the keyboard `listener` to `target`
  virtual void addKeyboardListener(RenderTarget* target, KeyboardListener* listener) = 0;

  /// @brief Remove the keyboard `listener` of `target`
  virtual void removeKeyboardListener(RenderTarget* target, KeyboardListener* listener) = 0;

  /// @brief Add the mouse `listener` to `target`
  virtual void addMouseListener(RenderTarget* target, MouseListener* listener) = 0;

  /// @brief Remove the mouse `listener` of `target`
  virtual void removeMouseListener(RenderTarget* target, MouseListener* listener) = 0;

  /// @brief Set if we run in debug-mode (needs to be set before creating windows/targets to take
  /// full effect)
  void setDebugMode(bool debugMode);

  /// @brief Check if we run in debug-mode
  bool debugMode() const;

  /// @brief Load the default vertex and fragment shaders and link them into a program of `target`
  ///
  /// @param defaultVertexShaderFile    File containing the default vertex shader
  /// @param defaultFragmentShaderFile  File containing the default fragment shader
  virtual void loadDefaultShaders(RenderTarget* target,
                                  const std::shared_ptr<File>& defaultVertexShaderFile,
                                  const std::shared_ptr<File>& defaultFragmentShaderFile) = 0;

  /// @brief Get the default vertex shader of `target`
  virtual const std::shared_ptr<Shader>& getDefaultVertexShader(RenderTarget* target) const = 0;

  /// @brief Get the default fragment shader of `target`
  virtual const std::shared_ptr<Shader>& getDefaultFragmentShader(RenderTarget* target) const = 0;

  /// @brief Get the default fragment shader of `target`
  virtual const std::shared_ptr<Program>& getDefaultProgram(RenderTarget* target) const = 0;

protected:
  RenderSystem(RenderSystemKind kind);

private:
  bool debugMode_;
};

} // namespace render

} // namespace sequoia

#endif
