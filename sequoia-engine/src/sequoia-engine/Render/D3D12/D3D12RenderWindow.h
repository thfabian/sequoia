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

#ifndef SEQUOIA_ENGINE_RENDER_D3D12_D3D12RENDERWINDOW_H
#define SEQUOIA_ENGINE_RENDER_D3D12_D3D12RENDERWINDOW_H

#include "sequoia-engine/Render/RenderWindow.h"

namespace sequoia {

namespace render {

/// @brief D3D12 render window
/// @ingroup d3d12
class SEQUOIA_API D3D12RenderWindow final : public RenderWindow {

  /// Is the window hidden?
  bool isHidden_;

  /// Is the window in full-screen mode?
  bool isFullscreen_;

  /// Is the window closed?
  bool isClosed_;

  /// Window geometry
  int height_, width_;

public:
  /// @brief Initialize window with given OpenGL context
  D3D12RenderWindow(const RenderWindow::WindowHint& hints);

  /// @brief Release window and destroy OpenGL context
  ~D3D12RenderWindow();

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

  static bool classof(const RenderTarget* target) {
    return target->getKind() == RK_D3D12RenderWindow;
  }
};

} // namespace render

} // namespace sequoia

#endif
