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

#ifndef SEQUOIA_MATH_MATH_H
#define SEQUOIA_MATH_MATH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/io.hpp>

namespace sequoia {

namespace math {

using namespace glm;

/// @brief Create a postion given a three dimensional vector
///
/// A position is given by a 4 dimensional vector `(x,y,z,w)` where `w = 1`
/// @ingroup math
inline vec4 makePosition(const vec3& vec) { return vec4(vec, 1); }

/// @brief Create a direction given a three dimensional vector
///
/// A direction is given by a 4 dimensional vector `(x,y,z,w)` where `w = 0`
/// @ingroup math
inline vec4 makeDirection(const vec3& vec) { return vec4(vec, 0); }

} // namespace math

} // namspace sequoia

#endif
