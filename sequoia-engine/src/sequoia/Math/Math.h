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

#include <blaze/math/DynamicMatrix.h>
#include <blaze/math/DynamicVector.h>
#include <blaze/math/StaticMatrix.h>
#include <blaze/math/StaticVector.h>

namespace sequoia {

namespace math {

namespace internal {
template <typename T>
using Vec2 = blaze::StaticVector<T, 2>;

template <typename T>
using Vec3 = blaze::StaticVector<T, 3>;

template <typename T>
using Vec4 = blaze::StaticVector<T, 4>;

template <typename T>
using VecX = blaze::DynamicVector<T>;

template <typename T>
using Mat2 = blaze::StaticMatrix<T, 2, 2>;

template <typename T>
using Mat3 = blaze::StaticMatrix<T, 3, 3>;

template <typename T>
using Mat4 = blaze::StaticMatrix<T, 4, 4>;

template <typename T>
using MatX = blaze::DynamicMatrix<T>;

} // namspace internal

} // namespace math

/// @addtogroup math
/// @{

/// @name Vector types
/// @{
using Vec2i = math::internal::Vec2<int>;
using Vec2f = math::internal::Vec2<float>;

using Vec3i = math::internal::Vec3<int>;
using Vec3f = math::internal::Vec3<float>;

using Vec4i = math::internal::Vec4<int>;
using Vec4f = math::internal::Vec4<float>;

using VecXi = math::internal::VecX<int>;
using VecXf = math::internal::VecX<float>;
/// @}

/// @name Matrix types
/// @{
using Mat2i = math::internal::Mat2<int>;
using Mat2f = math::internal::Mat2<float>;

using Mat3i = math::internal::Mat3<int>;
using Mat3f = math::internal::Mat3<float>;

using Mat4i = math::internal::Mat4<int>;
using Mat4f = math::internal::Mat4<float>;

using MatXi = math::internal::MatX<int>;
using MatXf = math::internal::MatX<float>;
/// @}

/// @}

} // namespace sequoia

#endif
