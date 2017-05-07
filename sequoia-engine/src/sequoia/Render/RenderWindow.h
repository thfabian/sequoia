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

#ifndef SEQUOIA_RENDER_RENDERWINDOW_H
#define SEQUOIA_RENDER_RENDERWINDOW_H

#include "sequoia/Render/RenderTarget.h"

namespace sequoia {

namespace render {

/// @brief Abstract base class of all RenderWindows
///
/// RenderWindows are created by the RenderSystem.
/// @ingroup render
class SEQUOIA_RENDER_API RenderWindow : public RenderTarget {
public:
  virtual ~RenderWindow() {}

  /// @brief Check if window is closed
  virtual bool isClosed() = 0;

  /// @brief Check if window is in fullscreen mode
  virtual bool isFullscreen() const = 0;

  /// @brief Initialize the window (this include context creation and binding for OpenGL windows)
  virtual void init() = 0;

  /// @name Window Geometry
  /// @{

  /// @brief Get the current width of the window
  virtual int getWidth() const = 0;

  /// @brief Get the current height of the window
  virtual int getHeight() const = 0;

  /// @}

  /// @name Rendering
  /// @{

  /// @brief Swaps the frame buffers to display the next frame
  virtual void swapBuffers() = 0;

  /// @brief Render one frame
  virtual void renderOneFrame() = 0;

  /// @}
};

} // namespace render

} // namespace sequoia

#endif
