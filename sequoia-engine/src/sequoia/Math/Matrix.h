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

#ifndef SEQUOIA_MATH_MATRIX_H
#define SEQUOIA_MATH_MATRIX_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Compiler.h"
#include <blaze/math/DynamicMatrix.h>
#include <blaze/math/StaticMatrix.h>

namespace sequoia {

namespace math {

template <typename T>
using Mat2 = blaze::StaticMatrix<T, 2, 2>;

template <typename T>
using Mat3 = blaze::StaticMatrix<T, 3, 3>;

template <typename T>
using Mat4 = blaze::StaticMatrix<T, 4, 4>;

template <typename T>
using MatX = blaze::DynamicMatrix<T>;

} // namespace math

/// @addtogroup math
/// @{

/// @name Matrix types
/// @{
using Mat2i = math::Mat2<int>;
using Mat2f = math::Mat2<float>;
using Mat2d = math::Mat2<double>;

using Mat3i = math::Mat3<int>;
using Mat3f = math::Mat3<float>;
using Mat3d = math::Mat3<double>;

using Mat4i = math::Mat4<int>;
using Mat4f = math::Mat4<float>;
using Mat4d = math::Mat4<double>;

using MatXi = math::MatX<int>;
using MatXf = math::MatX<float>;
using MatXd = math::MatX<double>;
/// @}

/// @}

namespace math {

/// @brief Convert matrix to identity matrix
///
/// @param matrix   Square matrix to modify
/// @ingroup math
template <class MatrixType>
SEQUOIA_INLINE void makeIdentity(MatrixType& matrix) noexcept {
  SEQUOIA_ASSERT(matrix.rows() == matrix.columns());
  matrix.reset();
  for(std::size_t i = 0; i < matrix.rows(); ++i)
    matrix(i, i) = typename MatrixType::ElementType(1);
}

} // namespace math

} // namespace sequoia

#endif
