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

#ifndef SEQUOIA_RENDER_CAMERA
#define SEQUOIA_RENDER_CAMERA

#include "sequoia/Core/Assert.h"
#include "sequoia/Math/Math.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/ViewFrustum.h"
#include "sequoia/Render/Viewport.h"
#include <string>

namespace sequoia {

namespace render {

/// @brief A viewpoint from which the scene will be rendered
///
/// Sequoia renders scenes from a camera viewpoint into a buffer of some sort, normally a window or
/// a texture (a subclass of RenderTarget). Each camera carries with it a style of rendering, e.g.
/// full textured, flat shaded, wireframe), field of view, rendering distances etc.
///
/// @ingroup render
class SEQUOIA_RENDER_API Camera : public ViewFrustum, public ViewportListener {
protected:
  /// Center of the scene (where the camera points to)
  math::vec4 center_;

  /// Eye of the camera (where the camera is located)
  math::vec4 eye_;

  /// Upward direction
  math::vec4 up_;

public:
  /// @brief Initialize the camera
  ///
  /// By default the camera is located at `(0, 0, -10)` at stares down at `(0, 0, 0)`.
  Camera();

  virtual ~Camera() {}

  /// @brief Get/Set the `eye` of the camera (where the camera is located)
  const math::vec4& getEye() const { return eye_; }
  void setEye(const math::vec4& eye) {
    SEQUOIA_ASSERT_MSG(eye.w == 1.0f, "not a valid position");
    eye_ = eye;
  }
  void setEye(const math::vec3& eye) { setEye(math::vec4(eye, 1.0f)); }

  /// @brief Get/Set the `center` of the camera (where the camera points to)
  const math::vec4& getCenter() const { return center_; }
  void setCenter(const math::vec4& center) {
    SEQUOIA_ASSERT_MSG(center.w == 1.0f, "not a valid position");
    center_ = center;
  }
  void setCenter(const math::vec3& center) { setCenter(math::vec4(center, 1.0f)); }

  /// @brief Get/Set the `up` direction of the camera (defines local coordiante system)
  const math::vec4& getUp() const { return up_; }
  void setUp(const math::vec4& up) {
    SEQUOIA_ASSERT_MSG(up.w == 0.0f, "not a valid direction");
    up_ = up;
  }
  void setUp(const math::vec3& up) { setUp(math::vec4(up, 0.0f)); }

  /// @brief Get `eye` to `center` offset (i.e `eye + offset = center`)
  math::vec4 getEyeToCenterOffset() const { return (center_ - eye_); }

  /// @brief The geometry of the associated viewport changed, we need to update our aspect ratio
  void viewportGeometryChanged(Viewport* viewport) override;

  /// @brief Convert to string
  std::string toString() const;
};

} // namespace render

} // namespace sequoia

#endif
