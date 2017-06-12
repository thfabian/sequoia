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
  math::vec3 center_;

  /// Eye of the camera (where the camera is located)
  math::vec3 eye_;

public:
  Camera();
  virtual ~Camera() {}

  /// @brief Get/Set the `eye` of the camera (where the camera is located)
  const math::vec3& getEye() const;
  void setEye(const math::vec3& eye);

  /// @brief Get/Set the `center` of the scene (where the camera points to)
  const math::vec3& getCenter() const;
  void setCenter(const math::vec3& center);

  /// @brief The geometry of the associated viewport changed, we need to update our aspect ratio
  void viewportGeometryChanged(Viewport* viewport) override;

  /// @brief Convert to string
  std::string toString() const;
};

} // namespace render

} // namespace sequoia

#endif
