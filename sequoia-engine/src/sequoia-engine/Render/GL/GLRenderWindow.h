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

#include "sequoia-engine/Render/GL/Native.h"
#include "sequoia-engine/Render/Input.h"
#include "sequoia-engine/Render/RenderWindow.h"

namespace sequoia {

namespace render {

/// @brief OpenGL render window
/// @ingroup gl
class SEQUOIA_API GLRenderWindow final : public RenderWindow,
                                         public InputEventListener,
                                         public NativeWindowListener {

  /// Native window handle
  std::shared_ptr<NativeWindow> nativeWindow_;

  /// Native input system handle
  std::shared_ptr<NativeInputSystem> nativeInputSystem_;

public:
  /// @brief Initialize window with given OpenGL context
  GLRenderWindow(std::shared_ptr<NativeGLContext> context);

  /// @brief Release window and destroy OpenGL context
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

  /// @copydoc RenderWindow::swapBuffers
  virtual void swapBuffers() override;

  /// @copydoc RenderWindow::setCursorMode
  virtual void setCursorMode(CursorModeKind mode) override;

  /// @copydoc RenderWindow::centerCursor
  virtual void centerCursor() override;

  /// @brief Get the associated OpenGL context of this window
  NativeInputSystem* getInputSystem();

  /// @copydoc InputEventListener::inputEventStart
  virtual void inputEventStart() override;

  /// @copydoc InputEventListener::inputEventStop
  virtual void inputEventStop() override;

  /// @copydoc InputEventListener::nativeWindowGeometryChanged
  virtual void nativeWindowGeometryChanged(NativeWindow* window) override;

  /// @copydoc InputEventListener::nativeWindowFocusChanged
  virtual void nativeWindowFocusChanged(NativeWindow* window) override;

  /// @brief Get the associated OpenGL context
  NativeGLContext* getContext() const;

  static bool classof(const RenderTarget* target) { return target->getKind() == RK_GLRenderWindow; }
};

} // namespace render

} // namespace sequoia

#endif
