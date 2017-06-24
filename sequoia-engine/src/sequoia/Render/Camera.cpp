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

#include "sequoia/Render/Camera.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Math/CoordinateSystem.h"

namespace sequoia {

namespace render {

Camera::Camera(const math::vec3& eye, const math::vec3& center, const math::vec3& up)
    : ViewFrustum(), modelMatrixIsDirty_(true) {
  lookAt(eye, center, up);
}

void Camera::lookAt(const math::vec3& eye, const math::vec3& center, const math::vec3& up) {
  math::vec3 eyeToCenter = center - eye;
  eyeToCenterDistance_ = math::length(eyeToCenter);

  // Note that by default the eye is located at the origin and the center is on the negatice Z-axis
  // The upvector is (0, 1, 0).
  //
  //        up
  //         ^   center                        u     f
  //         |  /                               \   /
  //         | /                                 \ /
  //        eye              ===>                 0------> s
  //
  //  1. Compute the direction between eye and center => f
  //  2. Compute the z-direction by `f x up` => s
  //  3. Compute the up-direction by `s x f` => u
  //  4. Local axes are `(s, u, -f) == `(x, y, z)`
  //
  const math::vec3 f = math::normalize(eyeToCenter);
  const math::vec3 s = math::normalize(math::cross(f, math::normalize(up)));
  const math::vec3 u = math::cross(s, f);
  math::mat3 localAxes(s, u, -f);

  setPosition(eye);
  setOrientation(math::normalize(math::quat(localAxes)));

  for(auto listener : getListeners<CameraPositionListener>()) {
    listener->cameraListenerPositionChanged(this);
    listener->cameraListenerRotationChanged(this);
  }
}

glm::vec3 Camera::getEye() const {
  // modelMat * (0, 0, 0, 1)
  return math::vec3(getModelMatrix()[3]);
}

glm::vec3 Camera::getCenter() const {
  // eye + modelMat * (0, 0, -1, 0) * eyeToCenterDistance
  return getEye() + math::vec3(getModelMatrix()[2] * (-eyeToCenterDistance_));
}

glm::vec3 Camera::getUp() const {
  // modelMat * (0, 1, 0, 0)
  return math::normalize(math::vec3(getModelMatrix()[math::CoordinateSystem::getUpIndex()]));
}

glm::mat4 Camera::getViewProjectionMatrix() const {
  return (math::perspective(math::Degree(getFieldOfViewY()).inRadians(), getAspectRatio(),
                            getZNearClipping(), getZFarClipping()) *
          math::lookAt(getEye(), getCenter(), getUp()));
}

void Camera::setPosition(const glm::vec3& position) {
  modelMatrixIsDirty_ = true;
  position_ = position;
}

void Camera::setOrientation(const glm::quat& orientation) {
  modelMatrixIsDirty_ = true;
  orientation_ = orientation;
}

void Camera::viewportGeometryChanged(Viewport* viewport) {
  setAspectRatio(float(viewport->getWidth()) / viewport->getHeight());
}

std::string Camera::toString() const {
  return core::format("Camera[\n"
                      "  fovy = %f,\n"
                      "  aspect = %f,\n"
                      "  zNear = %f,\n"
                      "  zFar = %f,\n"
                      "  eye = %s,\n"
                      "  center = %s,\n"
                      "  up = %s\n"
                      "]",
                      fovy_, aspect_, zNear_, zFar_, getEye(), getCenter(), getUp());
}

const math::mat4& Camera::getModelMatrix() const {
  if(modelMatrixIsDirty_)
    computeModelMatrix();
  return modelMatrix_;
}

void Camera::computeModelMatrix() const {
  // ModelMatrix = TranslationMatrix * RotatationMatrix
  modelMatrix_ = math::mat4(1.0f);
  modelMatrix_ = math::translate(modelMatrix_, position_);
  modelMatrix_ = modelMatrix_ * math::mat4_cast(orientation_);

  modelMatrixIsDirty_ = false;
}

} // namespace render

} // namespace sequoia
