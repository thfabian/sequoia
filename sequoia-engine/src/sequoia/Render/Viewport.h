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

#include "sequoia/Core/Listenable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderFwd.h"

namespace sequoia {

namespace render {

/// @brief Viewport Listeners
class SEQUOIA_RENDER_API ViewportListener {
public:
  /// @brief The geometry of the Viewport changed
  ///
  /// Default implementation does nothing
  virtual void viewportGeometryChanged(Viewport* viewport);
};

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
class SEQUOIA_RENDER_API Viewport : public Listenable<ViewportListener> {
  RenderTarget* target_;
  Camera* camera_;

  int x_;
  int y_;
  int width_;
  int height_;

public:
  /// @brief Initialize the view port
  Viewport(RenderTarget* target, int x, int y, int width, int height);
  virtual ~Viewport() {}

  /// @brief This register the camera as a Viewprot listener and immedialty notfies the camera
  /// with a `viewportGeometryChanged`
  void setCamera(Camera* camera);

  /// @brief Get the Camera
  Camera* getCamera() const;

  /// @brief Get the associated render target
  RenderTarget* getRenderTarget() const;

  /// @brief Update the geometry of the Viewport and inform all listeners
  void updateGeometry(int x, int y, int width, int height);

  /// @name Getter
  /// @{
  int getX() const;
  int getY() const;
  int getWidth() const;
  int getHeight() const;
  /// @}
};

} // namespace render

} // namespace sequoia

#endif
