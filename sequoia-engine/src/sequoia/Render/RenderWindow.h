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
  };

  /// @brief Set the mode of the cursor
  enum CursorModeKind {
    CK_Disabled, ///< This will hide the cursor and lock it to the specified window. It should be
                 ///  constantly re-centered
    CK_Hidden,   ///< Cursor becomes hidden when it is over a window
    CK_Normal    ///< Keep the cursor provided by the operating system
  };

  RenderWindow(RenderTargetKind kind) : RenderTarget(kind) {}
  virtual ~RenderWindow() {}

  /// @brief Initialize the window
  ///
  /// This may include context creation and binding.
  virtual void init() = 0;

  /// @brief Swaps the frame buffers to display the next frame
  virtual void swapBuffers() = 0;

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

  /// @brief Set the mode of the cursor
  virtual void setCursorMode(CursorModeKind mode) = 0;

  /// @brief Center the cursor in the middle of the window
  virtual void centerCursor() = 0;

  static bool classof(const RenderTarget* target) {
    return target->getKind() >= RK_RenderWindow && target->getKind() < RK_RenderWindowLast;
  }
};

} // namespace render

} // namespace sequoia

#endif
