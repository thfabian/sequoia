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

#include "sequoia/Core/Format.h"
#include "sequoia/Render/Camera.h"

namespace sequoia {

namespace render {

Camera::Camera()
    : ViewFrustum(), center_(0.f, 0.f, 0.f, 1.f), eye_(0.f, 0.f, -10.f, 1.f),
      up_(0.f, 1.f, 0.f, 0.f) {}

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
                      fovy_, aspect_, zNear_, zFar_, eye_, center_, up_);
}

} // namespace render

} // namespace sequoia
