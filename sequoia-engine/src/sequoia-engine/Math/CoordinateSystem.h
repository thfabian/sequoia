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

#ifndef SEQUOIA_ENGINE_MATH_COORDINATESYSTEM_H
#define SEQUOIA_ENGINE_MATH_COORDINATESYSTEM_H

#include "sequoia-engine/Math/Math.h"

namespace sequoia {

namespace math {

/// @brief Definition of the coordinate-system (Right-hand side)
///
/// @verbatim
///         y
///         |
///         |
///         |
///         0---------> x
///        /
///       /
///      z
/// @endverbatim
///
/// @ingroup math
struct CoordinateSystem {
  /// @brief Get the X unit-direction
  static inline vec3 X() noexcept { return vec3(1, 0, 0); }

  /// @brief Get the Y unit-direction
  static inline vec3 Y() noexcept { return vec3(0, 1, 0); }

  /// @brief Get the Z unit-direction
  static inline vec3 Z() noexcept { return vec3(0, 0, 1); }

  /// @brief Get the upward pointing direction
  static inline std::size_t getUpIndex() noexcept { return 1; }

  /// @brief Get the upward pointing direction
  static inline vec3 Up() noexcept { return CoordinateSystem::Y(); }
};

} // namespace math

} // namespace sequoia

#endif
