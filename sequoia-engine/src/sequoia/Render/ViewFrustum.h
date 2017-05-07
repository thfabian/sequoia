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

#ifndef SEQUOIA_RENDER_VIEWFRUSTUM_H
#define SEQUOIA_RENDER_VIEWFRUSTUM_H

#include "sequoia/Render/Export.h"

namespace sequoia {

namespace render {

/// @brief View Frustum
///
/// A view frustum is defined by 4 parameters:
///
/// @image html view_frustum.png
///
/// - @b fovy - The field of view parameter is basically the angle in between a plane passing
///   through the camera position as well as the top of your screen & another plane passing
///   through the camera position and the bottom of your screen. The bigger this angle is,
///   the more you can see of the world - but at the same time, the objects you can see will
///   become smaller.
///
/// - @b apect - The aspect ratio is your viewport's aspect ratio. In the graphic above, the
///   viewport is located at the the near clipping plane.
///
/// - @b zNear & @b zFar - The `zNear & zFar` values define the distance between the camera
///   position & the near and far clipping planes, respectively. Nothing that's closer to the camera
///   than `zNear` or farther away than `zFar` will be visible. Both values must be > 0, and
///   obviously, `zFar > zNear`.
///
/// @ingroup render
class SEQUOIA_RENDER_API ViewFrustum {
protected:
  double fovy_;
  double aspect_;
  double zNear_;
  double zFar_;

public:
  /// @brief Default constructor with
  ///   - @b fovy   : 45.0
  ///   - @b aspect : 1.3333
  ///   - @b zNear  : 10
  ///   - @b zFar   : 10000
  ViewFrustum() : fovy_(45.0), aspect_(4.0 / 3.0), zNear_(10.0), zFar_(10000.0) {}

  ViewFrustum(double fovy, double aspect, double zNear, double zFar)
      : fovy_(fovy), aspect_(aspect), zNear_(zNear), zFar_(zFar) {}

  /// @brief Get/Set the field of view
  /// @{
  void setFieldOfViewY(double fovy) { fovy_ = fovy; }
  double getFieldOfViewY() const { return fovy_; }
  /// @}

  /// @brief Get/Set the aspect ratio
  /// @{
  void setAspectRatio(double aspect) { aspect_ = aspect; }
  double getAspectRatio() const { return aspect_; }
  /// @}

  /// @brief Get/Set the near clipping plane
  /// @{
  void getZNearClipping(double zNear) { zNear_ = zNear; }
  double setZNearClipping() const { return zNear_; }
  /// @}

  /// @brief Get/Set the far clipping plane
  /// @{
  void getZFarClipping(double zFar) { zFar_ = zFar; }
  double setZFarClipping() const { return zFar_; }
  /// @}

protected:
  virtual void updateFrustum() = 0;
};

} // namespace render

} // namespace sequoia

#endif
