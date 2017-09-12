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

public:
  /// @brief Initialize glfw3
  glfw3NativeGLContext();

  /// @brief Terminate glfw3
  ~glfw3NativeGLContext();

  /// @brief Create OpenGL context *and* window
  void init(const RenderWindow::WindowHint& windowHints, Options* options) override;

  /// @brief Create a new context using `context` as a parent context
  void init(const std::shared_ptr<NativeGLContext>& context) override;

  /// @brief Makes the context current for the calling thread
  virtual void makeCurrent() override;

  /// @copydoc NativeGLContext::enableVSync
  virtual void enableVSync() override;

  /// @brief Get the parent context (if any)
  std::shared_ptr<NativeGLContext> getParent() const { return parent_; }

  /// @brief Get the glfw3 window
  inline GLFWwindow* getGLFWwindow() const { return window_; }

  static bool classof(const NativeGLContext* context) {
    return context->getKind() == NativeWindowSystemKind::NK_GLFW3;
  }
};

/// @brief glfw3 based OpenGL window
/// @ingroup gl
class glfw3NativeWindow final : public NativeWindow {

  /// Associated context
  std::shared_ptr<glfw3NativeGLContext> context_;

  /// Cache window geometry
  int width_, height_;

  /// Is the window focused?
  bool focused_;

  /// Singleton instance
  static glfw3NativeWindow* Instance;

public:
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

  /// @copydoc NativeWindow::isFocused
  bool isFocused() const override;

  /// @copydoc NativeWindow::getWidth
  int getWidth() const override;

  /// @copydoc NativeWindow::getHeight
  int getHeight() const override;

  /// @brief Resize the window to `width` x `height`
  void setGeometry(int width, int height);

  /// @brief Set the focus of the window
  void setFocus(bool focus);

  /// @brief Get the assocaited context
  NativeGLContext* getContext() const override { return context_.get(); }

  /// @brief Get the glfw3 window
  inline GLFWwindow* getGLFWwindow() const { return context_->getGLFWwindow(); }

  inline static glfw3NativeWindow* getInstance() { return glfw3NativeWindow::Instance; }

  static bool classof(const NativeWindow* window) {
    return window->getContext()->getKind() == NativeWindowSystemKind::NK_GLFW3;
  }
};

/// @brief glfw3 based input system
///
/// Note that there can be only one `glfw3NativeInputSystem` active at the time.
///
/// @ingroup gl
class glfw3NativeInputSystem final : public NativeInputSystem {

  /// Associated context
  std::shared_ptr<glfw3NativeWindow> window_;

  /// Previous mouse positions
  int prevPosX_, prevPosY_;

  /// Ignore the next mouse position event (by default the first mouse event is dropped as on Linux
  /// this sometimes has some wired coordiantes especially if launched on a secondary monitor)
  bool ignoreNextMousePosEvent_;

  /// Mode of the cursor
  RenderWindow::CursorModeKind cursorMode_;

  /// Singleton instance
  static glfw3NativeInputSystem* Instance;

public:
  glfw3NativeInputSystem(const std::shared_ptr<NativeWindow>& window, bool centerCursor);
  ~glfw3NativeInputSystem();

  /// @copydoc NativeInputSystem::pollEvents
  void pollEvents() override;

  /// @copydoc NativeInputSystem::centerCursor
  void centerCursor() override;

  /// @copydoc NativeInputSystem::setCursorMode
  void setCursorMode(RenderWindow::CursorModeKind mode) override;

  /// @copydoc NativeInputSystem::getCursorMode
  RenderWindow::CursorModeKind getCursorMode() override { return cursorMode_; }

  /// @copydoc NativeInputSystem::setCursorPosition
  void setCursorPosition(int xpos, int ypos) override;

  /// @copydoc NativeInputSystem::getWindow
  virtual NativeWindow* getWindow() const override { return window_.get(); }

  /// @brief Get the glfw3 window
  inline GLFWwindow* getGLFWwindow() const { return window_->getGLFWwindow(); }

  /// @name glfw callbacks
  /// @{
  void keyCallback(int key, int action, int mods) noexcept;
  void mouseButtonCallback(int button, int action, int mods) noexcept;
  void mousePositionCallback(int xpos, int ypos) noexcept;
  /// @}

  inline static glfw3NativeInputSystem* getInstance() { return glfw3NativeInputSystem::Instance; }

  static bool classof(const NativeWindow* window) {
    return window->getContext()->getKind() == NativeWindowSystemKind::NK_GLFW3;
  }
};

} // render

} // namespace sequoia

#endif
