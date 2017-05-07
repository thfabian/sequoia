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

#ifndef SEQUOIA_RENDER_GL_GLRENDERWINDOW_H
#define SEQUOIA_RENDER_GL_GLRENDERWINDOW_H

#include "sequoia/Render/RenderData.h"
#include "sequoia/Render/RenderWindow.h"
#include <string>
#include <unordered_map>
#include <vector>

struct GLFWwindow;

namespace sequoia {

namespace render {

/// @brief OpenGL render window
/// @ingroup gl
class SEQUOIA_RENDER_API GLRenderWindow : public RenderWindow {
  // std::vector<RenderData*> renderData_;

  GLFWwindow* window_;
  bool isFullscreen_;
  int windowWidth_;
  int windowHeight_;

public:
  /// @brief Initialize window
  ///
  /// This involves OpenGL context creation of the window.
  ///
  /// @throw RenderSystemInitException    Window creation failed
  GLRenderWindow(const std::string& title);

  /// @brief Release window and OpenGL context
  ~GLRenderWindow();

  /// @copydoc RenderWindow::isClosed
  virtual bool isClosed() override;

  /// @copydoc RenderWindow::isFullscreen
  virtual bool isFullscreen() const override;

  /// @copydoc RenderWindow::getWidth
  virtual int getWidth() const override;

  /// @copydoc RenderWindow::getHeight
  virtual int getHeight() const override;

  /// @copydoc RenderTarget::init
  virtual void init() override;

  /// @copydoc RenderTarget::swapBuffers
  virtual void swapBuffers() override;

  /// @copydoc RenderTarget::updateImpl
  virtual void update() override;

  /// @brief Get the window
  GLFWwindow* getGLFWwindow();

  /// @brief Called upon resizing the window
  void resizeCallback(int width, int height);
  static void resizeCallbackDispatch(GLFWwindow* window, int width, int height);

  /// @brief Static map of all GLFWwindows to their respective GLRenderWindows
  static std::unordered_map<GLFWwindow*, GLRenderWindow*> StaticWindowMap;

  static bool classof(const RenderTarget* target) { return target->getKind() == RK_GLRenderWindow; }
};

} // namespace render

} // namespace sequoia

#endif
