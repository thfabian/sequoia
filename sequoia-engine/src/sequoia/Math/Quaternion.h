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

#ifndef SEQUOIA_MATH_QUATERNION_H
#define SEQUOIA_MATH_QUATERNION_H

#include "sequoia/Math/Math.h"

namespace sequoia {

namespace math {

namespace internal {

template <class T>
class Quaternion {
  Vec4<T> data_;

public:
  Quaternion() = default;
  Quaternion(const Quaternion&) = default;
  Quaternion(Quaternion&&) = default;
  Quaternion(const Vec4<T>& data) : data_(data) {}
  Quaternion(Vec4<T>&& data) : data_(std::move(data)) {}

  Quaternion& operator=(const Quaternion&) = default;
  Quaternion& operator=(Quaternion&&) = default;
};

} // namespace internal

} // namespace math

/// @addtogroup math
/// @{

/// @name Quaternions
/// @{
using Quaternioni = math::internal::Quaternion<int>;
using Quaternionf = math::internal::Quaternion<float>;
/// @}

/// @}

} // namespace sequoia

#endif
