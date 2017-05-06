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

#include "sequoia/Render/RenderWindow.h"
#include <string>

struct GLFWwindow;

namespace sequoia {

namespace render {

/// @brief OpenGL render window
/// @ingroup render
class SEQUOIA_RENDER_API GLRenderWindow : public RenderWindow {
  GLFWwindow* window_;
  bool isFullscreen_;
  
public:
  /// @brief Initialize window
  ///
  /// This involves OpenGL context creation of the window.
  ///
  /// @throw RenderSystemInitException    Window creation failed
  GLRenderWindow(const std::string& title);

  /// @brief Terminate window
  ~GLRenderWindow();

  /// @copydoc RenderWindow::isClosed
  virtual bool isClosed() override;
  
  /// @copydoc RenderWindow::isFullscreen  
  virtual bool isFullscreen() const override;
  
  /// @copydoc RenderWindow::swapBuffers    
  virtual void swapBuffers() override;
  
  /// @brief Get the window
  GLFWwindow* getGLFWwindow();
};

} // namespace render

} // namespace sequoia

#endif
