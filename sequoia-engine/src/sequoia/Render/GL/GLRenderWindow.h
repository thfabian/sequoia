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

#ifndef SEQUOIA_RENDER_GL_GLRENDERWINDOW_H
#define SEQUOIA_RENDER_GL_GLRENDERWINDOW_H

#include "sequoia/Render/Input.h"
#include "sequoia/Render/RenderWindow.h"
#include <string>
#include <unordered_map>
#include <vector>

struct GLFWwindow;

namespace sequoia {

namespace render {

class GLRenderer;
class GLRenderSystem;
class GLInputSystem;

/// @brief OpenGL render window
/// @ingroup gl
class SEQUOIA_API GLRenderWindow final : public RenderWindow, public InputEventListener {
  GLRenderSystem* renderSystem_;
  GLFWwindow* window_;

  /// Short-hard or window geometry
  int windowWidth_;
  int windowHeight_;

  /// Mode of the cursor
  CursorModeKind mode_;

  std::unique_ptr<GLRenderer> renderer_;
  std::unique_ptr<GLInputSystem> inputSystem_;

public:
  /// @brief Initialize window
  ///
  /// This involves OpenGL context creation of the window.
  ///
  /// @throw RenderSystemInitException    Window creation failed
  GLRenderWindow(GLRenderSystem* renderSystem, const RenderWindow::WindowHint& windowHints);

  /// @brief Release window and OpenGL context
  ~GLRenderWindow();

  /// @copydoc RenderWindow::isHidden
  virtual bool isHidden() override;

  /// @copydoc RenderWindow::isClosed
  virtual bool isClosed() override;

  /// @copydoc RenderWindow::isFullscreen
  virtual bool isFullscreen() const override;

  /// @copydoc RenderWindow::getWidth
  virtual int getWidth() const override;

  /// @copydoc RenderWindow::getHeight
  virtual int getHeight() const override;

  /// @brief Initialize the OpenGL context by setting up the GLRenderer and initialize the
  /// GLInputSystem
  virtual void init() override;

  /// @copydoc RenderTarget::swapBuffers
  virtual void swapBuffers() override;

  /// @copydoc RenderTarget::update
  virtual void update() override;

  /// @copydoc RenderTarget::setCursorMode
  virtual void setCursorMode(CursorModeKind mode) override;

  /// @copydoc RenderTarget::centerCursor
  virtual void centerCursor() override;

  /// @brief Get the window
  GLFWwindow* getGLFWwindow();

  /// @brief Get the cursor mode
  CursorModeKind getCursorMode();

  /// @brief Get the associated OpenGL context of this window
  GLRenderer* getRenderer();

  /// @brief Get the associated OpenGL context of this window
  GLInputSystem* getInputSystem();

  /// @brief Called upon resizing the window
  void resizeCallback(int width, int height);

  /// @brief GLFW callbacks
  /// @{
  static void resizeCallbackDispatch(GLFWwindow* window, int width, int height);
  static void keyCallbackDispatch(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouseButtonCallbackDispatch(GLFWwindow* window, int button, int action, int mods);
  static void mousePositionCallbackDispatch(GLFWwindow* window, double xpos, double ypos);
  static void mouseEnterCallbackDispatch(GLFWwindow* window, int entered);
  /// @}

  /// @copydoc InputEventListener::inputEventStart
  virtual void inputEventStart() override;

  /// @copydoc InputEventListener::inputEventStop
  virtual void inputEventStop() override;

  /// @brief Static map of all GLFWwindows to their respective GLRenderWindows
  static std::unordered_map<GLFWwindow*, GLRenderWindow*> StaticWindowMap;

  static bool classof(const RenderTarget* target) { return target->getKind() == RK_GLRenderWindow; }
};

} // namespace render

} // namespace sequoia

#endif
