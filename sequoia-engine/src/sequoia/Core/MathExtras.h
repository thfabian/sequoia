//===-- sequoia/Core/MathExtras.h ---------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_MATHEXTRAS_H
#define SEQUOIA_CORE_MATHEXTRAS_H

#include <cstdint>

namespace sequoia {

namespace core {

/// @ingroup core
/// @{

/// @brief Returns the next power of two (in 64-bits) that is strictly greater than A. Returns zero
/// on overflow.
inline std::uint64_t nextPowerOf2(std::uint64_t A) {
  A |= (A >> 1);
  A |= (A >> 2);
  A |= (A >> 4);
  A |= (A >> 8);
  A |= (A >> 16);
  A |= (A >> 32);
  return A + 1;
}

/// @}

} // namespace core

} // namespace sequoia

#endif
