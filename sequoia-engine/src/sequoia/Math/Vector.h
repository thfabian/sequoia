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

#ifndef SEQUOIA_MATH_VECTOR_H
#define SEQUOIA_MATH_VECTOR_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Compiler.h"
#include <blaze/math/DynamicMatrix.h>
#include <blaze/math/StaticMatrix.h>

namespace sequoia {

namespace math {

template <typename T>
using Vec2 = blaze::StaticVector<T, 2>;

template <typename T>
using Vec3 = blaze::StaticVector<T, 3>;

template <typename T>
using Vec4 = blaze::StaticVector<T, 4>;

template <typename T>
using VecX = blaze::DynamicVector<T>;

} // namespace math

/// @addtogroup math
/// @{

/// @name Matrix types
/// @{
using Vec2i = math::Vec2<int>;
using Vec2f = math::Vec2<float>;
using Vec2d = math::Vec2<double>;

using Vec3i = math::Vec3<int>;
using Vec3f = math::Vec3<float>;
using Vec3d = math::Vec3<double>;

using Vec4i = math::Vec4<int>;
using Vec4f = math::Vec4<float>;
using Vec4d = math::Vec4<double>;

using VecXi = math::VecX<int>;
using VecXf = math::VecX<float>;
using VecXd = math::VecX<double>;
/// @}

/// @}

} // namespace sequoia

#endif
