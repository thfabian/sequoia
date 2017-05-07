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

#ifndef SEQUOIA_RENDER_VIEWPORT_H
#define SEQUOIA_RENDER_VIEWPORT_H

#include "sequoia/Render/Export.h"

namespace sequoia {

namespace render {

class Camera;
class RenderTarget;

/// @brief A Viewport connects a Camera with a RenderTarget
///
/// A view view port is defined by 4 parameters:
///
/// @image html view_port.jpg
///
/// - @b x, @b y - Specify the lower left corner of the viewport rectangle, in pixels. The initial
///   value is (0,0).
///
/// - @b width, @b height - Specify the width and height of the viewport.
///
/// A viewport is the meeting of a camera and a rendering surface - the camera renders the scene
/// from a viewpoint, and places its results into some subset of the render target, which may be
/// the whole surface or just a part of the surface. Each viewport has a single camera as source and
/// a single target as destination.
///
/// @ingroup render
class SEQUOIA_RENDER_API Viewport {
  Camera* camera_;
  RenderTarget* target_;

  int x_;
  int y_;
  int width_;
  int height_;

public:
  /// @brief Listener interface for viewport events
  class Listener {

    /// @brief Notification of when a new Camera is set to target listening Viewport
    virtual void viewportCameraChanged(Viewport* viewport);

    /// @brief Notification of when target listening Viewport's dimensions changed
    virtual void viewportDimensionsChanged(Viewport* viewport);
  };

  /// @brief Initialize the view port
  Viewport(Camera* camera, RenderTarget* target, int x, int y, int width, int height);

  /// @brief Get the associated camera
  Camera* getCamera() const { return camera_; }

  /// @brief Get the associated render target
  RenderTarget* getRenderTarget() const { return target_; }

  /// @name Getter/Setter
  /// @{
  void setX(int x) { x_ = x; }
  int getX() const { return x_; }

  void setY(int y) { y_ = y; }
  int getY() const { return y_; }

  void setWidth(int width) { width_ = width; }
  int getWidth() const { return width_; }

  void setHeight(int height) { height_ = height; }
  int getHeight() const { return height_; }
  /// @}
};

} // namespace render

} // namespace sequoia

#endif
