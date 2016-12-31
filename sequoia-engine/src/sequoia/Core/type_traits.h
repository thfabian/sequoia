//===-- sequoia/Core/type_traits.h --------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_TYPETRAITS_H
#define SEQUOIA_CORE_TYPETRAITS_H

#include "sequoia/Core/Core.h"
#include <type_traits>
#include <utility>

namespace sequoia {

namespace core {

/// @ingroup
/// @{

/// @brief This is a type trait that is used to determine whether a given type can be copied around
/// with memcpy instead of running ctors etc.
template <typename T>
struct isPodLike {

// std::is_trivially_copyable is available in libc++ with clang, libstdc++
// that comes with GCC 5.
#if(__has_feature(is_trivially_copyable) && defined(_LIBCPP_VERSION)) ||                           \
    (defined(__GNUC__) && __GNUC__ >= 5)
  // If the compiler supports the is_trivially_copyable trait use it, as it
  // matches the definition of isPodLike closely.
  static const bool value = std::is_trivially_copyable<T>::value;
#elif __has_feature(is_trivially_copyable)
  // Use the internal name if the compiler supports is_trivially_copyable but we
  // don't know if the standard library does. This is the case for clang in
  // conjunction with libstdc++ from GCC 4.x.
  static const bool value = __is_trivially_copyable(T);
#else
  // If we don't know anything else, we can (at least) assume that all non-class
  // types are PODs.
  static const bool value = !std::is_class<T>::value;
#endif
};

// std::pair's are pod-like if their elements are.
template <typename T, typename U>
struct isPodLike<std::pair<T, U>> {
  static const bool value = isPodLike<T>::value && isPodLike<U>::value;
};

/// @}

} // namespace core

} // namespace sequoia

#endif
