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
#include "sequoia-engine/Render/D3D12/D3D12.h"
#include "sequoia-engine/Render/Input.h"

namespace sequoia {

namespace render {

/// @brief D3D12 render window
/// @ingroup d3d12
class SEQUOIA_API D3D12RenderWindow final : public RenderWindow,
                                            public Listenable<KeyboardListener, MouseListener> {
  /// Modifierkey
  int keyModifier_;

  /// Window handle
  HWND hwnd_;

  /// Window geometry
  int height_, width_;

  /// Is the window in fullscreen?
  bool isFullscreen_;

  // Check if window was closed
  bool isClosed_;

public:
  /// @brief Initialize window
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

  /// @brief Poll the events
  void pollEvents();

  /// @brief Callback when a key is pressed/released
  void keyCallback(int key, int action, int mods) noexcept;

  /// @brief Callback when the window is resized
  void resizeCallback(int width, int height) noexcept;

  static bool classof(const RenderTarget* target) {
    return target->getKind() == RK_D3D12RenderWindow;
  }
};

} // namespace render

} // namespace sequoia

#endif
