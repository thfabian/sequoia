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

#ifndef SEQUOIA_CORE_HASHCOMBINE_H
#define SEQUOIA_CORE_HASHCOMBINE_H

#include <boost/functional/hash.hpp>

namespace sequoia {

namespace core {

/// @brief Called repeatedly to incrementally create a hash value from several variables
///
/// Note that the order of arguments matters, meaning:
/// `hash_combine(arg1, arg2) != hash_combine(arg2, arg1)`
///
/// @code
///   int a = 5;
///   int b = 2;
///   std::size_t hash = 0;
///   core::hash_combine(hash, a, b); // Use boost::hash to compute the individual hashes
/// @endcode
/// @ingroup support
/// @{
template <class T>
inline void hashCombine(std::size_t& seed, T&& arg) noexcept {
  boost::hash_combine(seed, arg);
}

template <class T, class... Args>
inline void hashCombine(std::size_t& seed, T&& arg, Args&&... args) noexcept {
  boost::hash_combine(seed, arg);
  core::hashCombine(seed, args...);
}
/// @}

} // namespace core

} // namespace sequoia

#endif
