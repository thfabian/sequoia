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

#include "sequoia/Math/Constants.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLCamera.h"
#include <cmath>

namespace sequoia {

namespace render {

void GLCamera::updateFrustum() {
  constexpr double pi = math::constants<double>::pi;

  // Calculate the distance from 0 of the y clipping plane i.e calculate position of clipper at
  // zNear
  const double fH = std::tan(fovy_ / (360 * pi)) * zNear_;

  // Calculate the distance from 0 of the x clipping plane based on the aspect ratio
  const double fW = fH * aspect_;

  glFrustum(-fW, fW, -fH, fH, zNear_, zFar_);
}

} // namespace sequoia

} // namespace sequoia
