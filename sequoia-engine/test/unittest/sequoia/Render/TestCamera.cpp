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
#include "sequoia/Unittest/Test.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::render;

namespace {

TEST(CameraTest, Initialization) {
  {
    Camera camera(math::vec3(0, 0, -10), math::vec3(0, 0, 0));
    EXPECT_VEC3_NEAR(camera.getEye(), math::vec3(0, 0, -10), 1e-05f);
    EXPECT_VEC3_NEAR(camera.getCenter(), math::vec3(0, 0, 0), 1e-05f);
    EXPECT_VEC3_NEAR(camera.getUp(), math::vec3(0, 1, 0), 1e-05f);
  }

  {
    Camera camera(math::vec3(0, 0, 10), math::vec3(0, 0, 0));
    EXPECT_VEC3_NEAR(camera.getEye(), math::vec3(0, 0, 10), 1e-05f);
    EXPECT_VEC3_NEAR(camera.getCenter(), math::vec3(0, 0, 0), 1e-05f);
    EXPECT_VEC3_NEAR(camera.getUp(), math::vec3(0, 1, 0), 1e-05f);
  }

  {
    Camera camera(math::vec3(0, 0, 0), math::vec3(-5, -5, -5));
    EXPECT_VEC3_NEAR(camera.getEye(), math::vec3(0, 0, 0), 1e-05f);
    EXPECT_VEC3_NEAR(camera.getCenter(), math::vec3(-5, -5, -5), 1e-05f);
  }
}

} // anonymous namespace
