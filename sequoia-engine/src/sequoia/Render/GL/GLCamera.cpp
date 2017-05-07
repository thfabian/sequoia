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

#include <iostream>

namespace sequoia {

namespace render {

namespace {

template <typename T>
SEQUOIA_INLINE void makeIdentity(T m[16]) {
  m[0 + 4 * 0] = 1;
  m[0 + 4 * 1] = 0;
  m[0 + 4 * 2] = 0;
  m[0 + 4 * 3] = 0;
  m[1 + 4 * 0] = 0;
  m[1 + 4 * 1] = 1;
  m[1 + 4 * 2] = 0;
  m[1 + 4 * 3] = 0;
  m[2 + 4 * 0] = 0;
  m[2 + 4 * 1] = 0;
  m[2 + 4 * 2] = 1;
  m[2 + 4 * 3] = 0;
  m[3 + 4 * 0] = 0;
  m[3 + 4 * 1] = 0;
  m[3 + 4 * 2] = 0;
  m[3 + 4 * 3] = 1;
}

template <typename T>
SEQUOIA_INLINE void normalize(T v[3]) {
  const T r = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  if(r == 0.0)
    return;
  v[0] /= r;
  v[1] /= r;
  v[2] /= r;
}

template <typename T>
SEQUOIA_INLINE void cross(T v1[3], T v2[3], T result[3]) {
  result[0] = v1[1] * v2[2] - v1[2] * v2[1];
  result[1] = v1[2] * v2[0] - v1[0] * v2[2];
  result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

// Derived from gluPerspective
static void computeProjectionMatrix(double fovy, double aspect, double zNear, double zFar) {
  constexpr double pi = math::constants<double>::pi;
  double m[4][4];

  const double radians = fovy / 2.0 * pi / 180.0;
  const double deltaZ = zFar - zNear;
  const double sine = std::sin(radians);

  if((deltaZ == 0) || (sine == 0) || (aspect == 0))
    return;

  const double cotangent = std::cos(radians) / sine;

  makeIdentity(&m[0][0]);
  m[0][0] = cotangent / aspect;
  m[1][1] = cotangent;
  m[2][2] = -(zFar + zNear) / deltaZ;
  m[2][3] = -1;
  m[3][2] = -2 * zNear * zFar / deltaZ;
  m[3][3] = 0;
  glMultMatrixd(&m[0][0]);
}

// Derived from gluLookAt
static void computeModelViewMatrix(double eyex, double eyey, double eyez, double centerx,
                                   double centery, double centerz, double upx, double upy,
                                   double upz) {
  float m[4][4];
  float forward[3], side[3], up[3];

  forward[0] = centerx - eyex;
  forward[1] = centery - eyey;
  forward[2] = centerz - eyez;

  up[0] = upx;
  up[1] = upy;
  up[2] = upz;

  normalize(forward);

  // Side = forward x up
  cross(forward, up, side);
  normalize(side);

  // Recompute up as: up = side x forward
  cross(side, forward, up);

  makeIdentity(&m[0][0]);
  m[0][0] = side[0];
  m[1][0] = side[1];
  m[2][0] = side[2];

  m[0][1] = up[0];
  m[1][1] = up[1];
  m[2][1] = up[2];

  m[0][2] = -forward[0];
  m[1][2] = -forward[1];
  m[2][2] = -forward[2];

  glMultMatrixf(&m[0][0]);
  glTranslated(-eyex, -eyey, -eyez);
}

} // anonymous namespace

GLCamera::GLCamera(const Vec3f& up) : Camera(up) {}

GLCamera::~GLCamera() {}

void GLCamera::updateModelViewMatrix() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  computeModelViewMatrix(eye_[0], eye_[1], eye_[2], center_[0], center_[1], center_[2], up_[0],
                         up_[1], up_[2]);
}

void GLCamera::updateProjectionMatrix(Viewport* viewport) {
  glViewport(viewport->getX(), viewport->getY(), viewport->getWidth(), viewport->getHeight());
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
  // computeProjectionMatrix(fovy_, aspect_, zNear_, zFar_);
}

} // namespace render

} // namespace sequoia
