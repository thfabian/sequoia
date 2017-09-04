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

#ifndef SEQUOIA_CORE_HASH_H
#define SEQUOIA_CORE_HASH_H

#include <functional>
#include <type_traits>
#include <utility>

namespace sequoia {

namespace core {

namespace internal {

inline void hashCombineImpl(std::size_t& seed, std::size_t hash) noexcept {
  seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <class T>
inline std::size_t hashImpl(const T& value) noexcept;

template <bool IsEnum, bool IsArray>
struct Hasher {
  template <class T>
  static inline std::size_t hash(const T& value) noexcept {
    std::hash<T> hasher;
    return hasher(value);
  }
};

template <>
struct Hasher<true, false> {
  template <class T>
  static inline std::size_t hash(const T& value) noexcept {
    using UnderlyingType = std::underlying_type_t<T>;
    std::hash<UnderlyingType> hasher;
    return hasher(static_cast<UnderlyingType>(value));
  }
};

template <>
struct Hasher<false, true> {
  template <class T, std::size_t N>
  static inline std::size_t hash(const T (&value)[N]) noexcept {
    std::size_t seed = 0;
    for(std::size_t i = 0; i < N; ++i)
      hashCombineImpl(seed, hashImpl(value[i]));
    return seed;
  }
};

template <class T>
inline std::size_t hashImpl(const T& value) noexcept {
  return Hasher<std::is_enum<T>::value, std::is_array<T>::value>::hash(value);
}

} // namespace internal

inline void hashCombine(std::size_t& seed) noexcept { (void)seed; }

/// @brief Called repeatedly to incrementally create a hash value from several variables
///
/// Note that the order of arguments matters, meaning:
/// `hashCombine(arg1, arg2) != hashCombine(arg2, arg1)`
///
/// @code
///   int a = 5;
///   int b = 2;
///   std::size_t hash = 0;
///   core::hashCombine(hash, a, b); // Uses std::hash<T> to compute the individual hashes
/// @endcode
///
/// @ingroup core
template <class T, class... Args>
inline void hashCombine(std::size_t& seed, const T& arg, const Args&... args) noexcept {
  internal::hashCombineImpl(seed, internal::hashImpl(arg));
  hashCombine(seed, args...);
}

/// @brief Compute a hash of the given `values`
///
/// Multiple values are combined via `hashCombine`.
/// This requires a specialization of `std::hash<T>` foreach `T` in `Args`.
///
/// @ingroup core
template <class... Args>
inline std::size_t hash(Args&&... values) noexcept {
  std::size_t seed = 0;
  hashCombine(seed, std::forward<Args>(values)...);
  return seed;
}

} // namespace core

} // namespace sequoia

/// @brief Helper macro to declare a specialization for `std::hash<type>`
///
/// @param type     Type to specialize for (this needs to be fully qualified)
/// @param value    Name of the variable which is passed to `std::hash<type>::operator()`
/// @param ...      Members used to compute the hash
///
/// Note this macro has to be used outside *any* namespaces.
///
/// @code{.cpp}
///   namespace foo {
///
///   struct Foo {
///     int MyInteger;
///     std::string MyString;
///   };
///
///   }
///
///   // We compute the hash by combining the hash of MyInteger and MyString member
///   SEQUOIA_DECLARE_STD_HASH(foo::Foo, value, value.MyInteger, value.MyString);
/// @endcode
///
/// @ingroup core
#define SEQUOIA_DECLARE_STD_HASH(type, value, ...)                                                 \
  namespace std {                                                                                  \
  template <>                                                                                      \
  struct hash<type> {                                                                              \
    std::size_t operator()(const type& value) const { return sequoia::core::hash(__VA_ARGS__); }   \
  };                                                                                               \
  }

#endif
