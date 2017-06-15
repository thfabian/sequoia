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

#ifndef SEQUOIA_RENDER_RENDERWINDOW_H
#define SEQUOIA_RENDER_RENDERWINDOW_H

#include "sequoia/Render/RenderTarget.h"

namespace sequoia {

namespace render {

/// @brief Abstract base class of all RenderWindows
/// @ingroup render
class SEQUOIA_API RenderWindow : public RenderTarget {
public:
  /// @brief Hints used to construct the window
  struct WindowHint {
    enum WindowModeKind { WK_Window, WK_WindowedFullscreen, WK_Fullscreen };

    /// Title of the window
    std::string Title = "<unknown>";

    /// Width of the window (only relevant in window mode)
    int Width = 1280;

    /// Height of the window (only relevant in window mode)
    int Height = 960;

    /// Monitor to use
    int Monitor = 0;

    /// Window mode
    WindowModeKind WindowMode = WK_Window;

    /// Use hidden window?
    bool HideWindow = false;

    /// Full-scene anti-aliasing
    int MSAA = 0;

    /// OpenGL major version
    int GLMajorVersion = 3;

    /// OpenGL minor version
    int GLMinorVersion = 3;

    /// Is IO enabled (i.e captures Keyboard and Mouse events)?
    bool IOEnabled = true;
  };

  RenderWindow(RenderTargetKind kind) : RenderTarget(kind) {}

  virtual ~RenderWindow() {}

  /// @brief Check if window is hidden
  virtual bool isHidden() = 0;

  /// @brief Check if window is closed
  virtual bool isClosed() = 0;

  /// @brief Check if window is in fullscreen mode
  virtual bool isFullscreen() const = 0;

  /// @brief Get the current width of the window
  virtual int getWidth() const = 0;

  /// @brief Get the current height of the window
  virtual int getHeight() const = 0;

  /// @copydoc RenderTarget::init
  virtual void init() override = 0;

  /// @copydoc RenderTarget::swapBuffers
  virtual void swapBuffers() override = 0;

  /// @copydoc RenderTarget::update
  virtual void update() override = 0;

  static bool classof(const RenderTarget* target) {
    return target->getKind() >= RK_RenderWindow && target->getKind() < RK_RenderWindowLast;
  }
};

} // namespace render

} // namespace sequoia

#endif
