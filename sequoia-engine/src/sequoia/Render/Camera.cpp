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
#include <iostream>

namespace sequoia {

namespace render {

Camera::Camera(const Vec3f& up) : ViewFrustum(), up_(up) {}

const Vec3f& Camera::getUp() const { return up_; }

void Camera::viewportGeometryChanged(Viewport* viewport) {
  setAspectRatio(viewport->getWidth() / viewport->getHeight());
  updateProjectionMatrix(viewport);
}

const Vec3f& Camera::getEye() const { return eye_; }

void Camera::setEye(const Vec3f& eye) {
  eye_[0] = eye[0];
  eye_[1] = eye[1];
  eye_[2] = eye[2];
}

const Vec3f& Camera::getCenter() const { return center_; }

void Camera::setCenter(const Vec3f& center) {
  center_[0] = center[0];
  center_[1] = center[1];
  center_[2] = center[2];
}

} // namespace render

} // namespace sequoia
