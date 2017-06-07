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

#include "sequoia/Render/Camera.h"

namespace sequoia {

namespace render {

const math::vec3 Camera::Up(0, 1, 0);

Camera::Camera() : ViewFrustum() {}

void Camera::viewportGeometryChanged(Viewport* viewport) {
  setAspectRatio(float(viewport->getWidth()) / viewport->getHeight());
}

const math::vec3& Camera::getEye() const { return eye_; }

void Camera::setEye(const math::vec3& eye) { eye_ = eye; }

const math::vec3& Camera::getCenter() const { return center_; }

void Camera::setCenter(const math::vec3& center) { center_ = center; }

} // namespace render

} // namespace sequoia
