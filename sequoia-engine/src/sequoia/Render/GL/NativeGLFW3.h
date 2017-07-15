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

#ifndef SEQUOIA_RENDER_GL_NATIVEGLFW3_H
#define SEQUOIA_RENDER_GL_NATIVEGLFW3_H

#include "sequoia/Render/GL/Native.h"
#include <unordered_map>

struct GLFWwindow;

namespace sequoia {

namespace render {

/// @brief glfw3 based OpenGL context (and window)
/// @ingroup gl
class glfw3NativeGLContext final : public NativeGLContext {

  /// glfw3 window (and context) handle
  mutable GLFWwindow* window_;

  /// Parent context (if any)
  std::shared_ptr<NativeGLContext> parent_;

  /// Number of initialized contexts (the first context initializes glfw3 and the last one will
  /// terminate it)
  static int NumContexts;

  /// Static map of all GLFWwindows to their respective NativeGLContext
  static std::unordered_map<GLFWwindow*, glfw3NativeGLContext*> ContextMap;

public:
  /// @brief Initialize glfw3
  glfw3NativeGLContext();

  /// @brief Terminate glfw3
  ~glfw3NativeGLContext();

  /// @brief Create OpenGL context *and* window
  void init(const RenderWindow::WindowHint& windowHints) override;

  /// @brief Create a new context using `context` as a parent context
  void init(const std::shared_ptr<NativeGLContext>& context) override;

  /// @brief Get the parent context (if any)
  std::shared_ptr<NativeGLContext> getParent() const { return parent_; }

  /// @brief Get the window
  inline GLFWwindow* getGLFWwindow() const { return window_; }

  /// @brief Get the context given the `GLFWwindow`
  static glfw3NativeGLContext* getNativeGLContext(GLFWwindow* window);

  static bool classof(const NativeGLContext* context) { return context->getKind() == NK_GLFW3; }
};

/// @brief glfw3 based OpenGL window
/// @ingroup gl
class glfw3NativeWindow final : public NativeWindow {

  /// Associated context
  std::shared_ptr<glfw3NativeGLContext> context_;

  /// Static map of all GLFWwindows to their respective NativeWindows
  static std::unordered_map<GLFWwindow*, glfw3NativeWindow*> WindowMap;

public:
  /// @brief Initialize window with `context`
  glfw3NativeWindow(const std::shared_ptr<NativeGLContext>& context);
  ~glfw3NativeWindow();

  /// @copydoc NativeWindow::swapBuffers
  void swapBuffers() override;

  /// @copydoc NativeWindow::isHidden
  bool isHidden() override;

  /// @copydoc NativeWindow::isClosed
  bool isClosed() override;

  /// @copydoc NativeWindow::isFullscreen
  bool isFullscreen() const override;

  /// @copydoc NativeWindow::getWidth
  int getWidth() const override;

  /// @copydoc NativeWindow::getHeight
  int getHeight() const override;

  /// @brief Get the assocaited context
  NativeGLContext* getContext() const override { return context_.get(); }
  
  /// @brief Get the context given the `GLFWwindow`
  static glfw3NativeWindow* getNativeWindow(GLFWwindow* window);

  static bool classof(const NativeWindow* window) {
    return window->getContext()->getKind() == NK_GLFW3;
  }
};

} // render

} // namespace sequoia

#endif
