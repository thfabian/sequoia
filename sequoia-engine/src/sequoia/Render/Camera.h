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

#ifndef SEQUOIA_RENDER_CAMERA
#define SEQUOIA_RENDER_CAMERA

#include "sequoia/Math/Vector.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/ViewFrustum.h"
#include "sequoia/Render/Viewport.h"

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
  /// Eye of the camera (where the camera is located)
  Vec3f eye_;

  /// Center of the scene (where the camera points to)
  Vec3f center_;

  /// Up vector
  Vec3f up_;

public:
  Camera(const Vec3f& up);
  virtual ~Camera() {}

  /// @brief Get/Set the `eye` of the camera (where the camera is located)
  const Vec3f& getEye() const;
  void setEye(const Vec3f& eye);

  /// @brief Get/Set the `center` of the scene (where the camera points to)
  const Vec3f& getCenter() const;
  void setCenter(const Vec3f& center);

  /// @brief Get `up` vector
  const Vec3f& getUp() const;

  /// @brief Update the model view matrix
  virtual void updateModelViewMatrix() = 0;

  /// @copydoc ViewFrustum::updateProjectionMatrix
  virtual void updateProjectionMatrix(Viewport* viewport) override = 0;

  /// @brief The geometry of the associated viewport changed, we need to update our aspect ratio
  void viewportGeometryChanged(Viewport* viewport) override;
};

} // namespace render

} // namespace sequoia

#endif
