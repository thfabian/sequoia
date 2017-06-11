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

#ifndef SEQUOIA_MATH_CONSTANTS_H
#define SEQUOIA_MATH_CONSTANTS_H

namespace sequoia {

namespace math {

/// @brief Constexpr version of common constants
///
/// Constants are taken from `<boost/math/constants/constants.hpp>`.
///
/// @see http://www.boost.org/doc/libs/1_60_0/libs/math/doc/html/math_toolkit/constants.html
/// @ingroup math
template <class T>
struct constants {

  /// @brief pi
  static constexpr T pi = 3.141592653589793238462643383279502884e+00;

  /// @brief pi/2
  static constexpr T half_pi = 1.570796326794896619231321691639751442e+00;

  /// @brief pi/3
  static constexpr T third_pi = 1.047197551196597746154214461093167628e+00;

  /// @brief 2*pi
  static constexpr T two_pi = 6.283185307179586476925286766559005768e+00;

  /// @brief 2/3*pi
  static constexpr T two_thirds_pi = 2.094395102393195492308428922186335256e+00;

  /// @brief 3/4*pi
  static constexpr T three_quarters_pi = 2.356194490192344928846982537459627163e+00;

  /// @brief 4/3*pi
  static constexpr T four_thirds_pi = 4.188790204786390984616857844372670512e+00;
};

} // namespace math

} // namespace sequoia

#endif
