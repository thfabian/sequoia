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

/// @addtogroup math
/// @{

/// @name Vector types
/// @{
using Vec2i = blaze::StaticVector<int, 2>;
using Vec2f = blaze::StaticVector<float, 2>;

using Vec3i = blaze::StaticVector<int, 3>;
using Vec3f = blaze::StaticVector<float, 3>;

using Vec4i = blaze::StaticVector<int, 4>;
using Vec4f = blaze::StaticVector<float, 4>;

using VecXi = blaze::DynamicVector<int>;
using VecXf = blaze::DynamicVector<float>;
/// @}

/// @name Vector types
/// @{
using Mat2i = blaze::StaticMatrix<int, 2, 2>;
using Mat2f = blaze::StaticMatrix<float, 2, 2>;

using Mat3i = blaze::StaticMatrix<int, 3, 3>;
using Mat3f = blaze::StaticMatrix<float, 3, 3>;

using Mat4i = blaze::StaticMatrix<int, 4, 4>;
using Mat4f = blaze::StaticMatrix<float, 4, 4>;

using MatXi = blaze::DynamicMatrix<int>;
using MatXf = blaze::DynamicMatrix<float>;
/// @}

/// @}

} // namespace math

} // namespace sequoia

#endif
