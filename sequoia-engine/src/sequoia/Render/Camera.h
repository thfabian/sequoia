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
#include <memory>
#include <string>

namespace sequoia {

namespace render {

/// @brief A viewpoint from which the scene will be rendered
///
/// Sequoia renders scenes from a camera viewpoint into a buffer of some sort, normally a window or
/// a texture (a subclass of RenderTarget).
/// @ingroup render
class SEQUOIA_RENDER_API Camera : public ViewFrustum, public ViewportListener {
protected:
  /// Translation of the camera
  math::vec3 position_;

  /// Orientation of the camera
  math::quat orientation_;

  /// Distance from eye to center
  float eyeToCenterDistance_;

  /// Model matrix (used to compute eye, center and up)
  mutable math::mat4 modelMatrix_;
  mutable bool modelMatrixIsDirty_;

public:
  /// @brief Initialize the camera
  ///
  /// By default the camera is located at `(0, 0, 10)` and stares down at `(0, 0, 0)`.
  Camera(const math::vec3& eye = math::vec3(2, 2, 2),
         const math::vec3& center = math::vec3(0, 0, 0), math::vec3 up = math::vec3(0, 1, 0));

  virtual ~Camera() {}

  /// @brief Get/Set the `eye` of the camera (where the camera is located)
  math::vec3 getEye() const;

  /// @brief Get/Set the `center` of the camera (where the camera points to)
  math::vec3 getCenter() const;

  /// @brief Get/Set the `up` direction of the camera (defines local coordiante system)
  math::vec3 getUp() const;

  /// @brief Get the view projection matrix
  ///
  /// This returns `matViewProj = matProj * matView` where `matProj` is the projection matrix and
  /// `matView` is the camera view matrix. This call is formally equivalent to:
  ///
  /// @code{.cpp}
  /// math::mat4 matProj = math::perspective(getFieldOfViewY(), getAspectRatio(),
  ///                                        getZNearClipping(), getZFarClipping());
  /// math::mat4 matView = math::lookAt(getEye(), getCenter(), getUp());
  /// math::mat4 matViewProj = matProj * matView;
  /// @endcode
  math::mat4 getViewProjectionMatrix() const;

  /// @brief Set the position of the camera (equivalent to `setEye()`)
  void setPosition(const math::vec3& position) {
    modelMatrixIsDirty_ = true;
    position_ = position;
  }
  /// @brief Set the orientation of the camera (in world space)
  void setOrientation(const math::quat& orientation) {
    modelMatrixIsDirty_ = true;
    orientation_ = orientation;
  }

  /// @brief Get the position (this is equivalent to `getEye()`)
  const math::vec3& getPosition() const { return position_; }

  /// @brief Get the orientation
  const math::quat& getOrientation() const { return orientation_; }

  /// @brief The geometry of the associated viewport changed, we need to update our aspect ratio
  void viewportGeometryChanged(Viewport* viewport) override;

  /// @brief Convert to string
  std::string toString() const;

private:
  /// @brief Get the model matrix
  const math::mat4& getModelMatrix() const;

  /// @brief Compute the model matrix
  void computeModelMatrix() const;
};

} // namespace render

} // namespace sequoia

#endif
