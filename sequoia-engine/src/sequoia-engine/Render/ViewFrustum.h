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

#ifndef SEQUOIA_RENDER_VIEWFRUSTUM_H
#define SEQUOIA_RENDER_VIEWFRUSTUM_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Math/Math.h"

namespace sequoia {

namespace render {

class Viewport;

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
class SEQUOIA_API ViewFrustum {
public:
  /// @brief Default constructor with
  ///   - @b fovy   : 45.0
  ///   - @b aspect : 1.3333
  ///   - @b zNear  : 1.0
  ///   - @b zFar   : 10000.0
  ViewFrustum() : fovy_(45.0f), aspect_(4.0f / 3.0f), zNear_(1.0f), zFar_(10000.0f) {}

  ViewFrustum(double fovy, double aspect, double zNear, double zFar)
      : fovy_(fovy), aspect_(aspect), zNear_(zNear), zFar_(zFar) {}

  virtual ~ViewFrustum() {}

  /// @brief Specifies the field of view angle, in  @b degrees, in the `y `direction
  /// @{
  void setFieldOfViewY(math::Degree fovy) { fovy_ = fovy; }
  math::Degree getFieldOfViewY() const { return fovy_; }
  /// @}

  /// @brief Specifies the aspect ratio that determines the field of view in the `x` direction. The
  /// aspect ratio is the ratio of `x` (width) to `y` (height)
  /// @{
  void setAspectRatio(float aspect) { aspect_ = aspect; }
  float getAspectRatio() const { return aspect_; }
  /// @}

  /// @brief Specifies the distance from the viewer to the near clipping plane (always positive)
  /// @{
  void setZNearClipping(float zNear) { zNear_ = zNear; }
  float getZNearClipping() const { return zNear_; }
  /// @}

  /// @brief Specifies the distance from the viewer to the far clipping plane (always positive)
  /// @{
  void setZFarClipping(float zFar) { zFar_ = zFar; }
  float getZFarClipping() const { return zFar_; }
  /// @}

protected:
  math::Degree fovy_;
  float aspect_;
  float zNear_;
  float zFar_;
};

} // namespace render

} // namespace sequoia

#endif
