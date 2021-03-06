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

#ifndef SEQUOIA_ENGINE_CORE_STRINGUTIL_H
#define SEQUOIA_ENGINE_CORE_STRINGUTIL_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringRef.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>

namespace sequoia {

namespace core {

/// @brief Indent a string by the specified number of spaces
/// @ingroup core
SEQUOIA_API extern std::string indent(const std::string& string, int amount = 2);

/// @brief Execute `functor` for each line in `string`
/// @ingroup core
SEQUOIA_API extern void forEachLine(const std::string& string,
                                    std::function<void(StringRef)> functor);

/// @brief Convert a time value in milliseconds into a human-readable string
/// @ingroup core
SEQUOIA_API extern std::string timeString(double time, bool precise = false);

/// @brief Convert a pointer to string
/// @ingroup core
template <class T>
inline std::string ptrToStr(const T& ptr) noexcept {
  static_assert(std::is_pointer<T>::value, "T is not a poitner type");
  return core::format("{:p}", static_cast<const void*>(ptr));
}

namespace internal {

template <class T>
struct StdToString {
  std::string operator()(const T& t) { return std::to_string(t); }
};

template <>
struct StdToString<std::string> {
  std::string operator()(const std::string& t) { return t; }
};

template <>
struct StdToString<const char*> {
  std::string operator()(const char* t) { return t; }
};

} // namespace internal

/// @brief Wrapper for `std::to_string` which is able to deal with `const char*` and `std::string`
/// arguments
template <class T>
inline std::string toString(const T& value) {
  return internal::StdToString<T>()(value);
}

/// @brief Converts range based containers to string
///
/// @b Example:
/// @code
///   std::vector<int> v = {1,2,3,4,5};
///   std::cout << RangeToString()(v); // [1, 2, 3, 4, 5]
/// @endcode
///
/// @ingroup core
class RangeToString {
  const char* delim_;
  const char* start_;
  const char* end_;

public:
  /// @brief Convert each element to string using `std::to_string`
  struct DefaultStringifyFunctor {
    template <class T>
    std::string operator()(const T& t) {
      return internal::StdToString<T>()(t);
    }
  };

  RangeToString(const char* delim = ", ", const char* start = "[", const char* end = "]")
      : delim_(delim), start_(start), end_(end) {}

  /// @brief Convert a `Range` to string (elements of the Range are converted to a string via
  /// `std::to_string`)
  ///
  /// Optionally, an already existing `stringstream` can be provided in which case the `range` is
  /// directly streamed into the stream and an empty string is returned.
  template <class Range>
  inline std::string operator()(Range&& range) {
    return this->operator()(std::forward<Range>(range), DefaultStringifyFunctor());
  }

  /// @brief Convert a `Range` to string where each element will be converted to string using the
  /// `stringify` functor
  ///
  /// Optionally, an already existing `stringstream` can be provided in which case the `range` is
  /// directly streamed into the stream and an empty string is returned.
  template <class Range, class StrinfigyFunctor>
  inline std::string operator()(Range&& range, StrinfigyFunctor&& stringify) {
    std::stringstream ss;

    ss << start_;
    auto it = std::begin(range), end = std::end(range);
    const std::size_t size = std::distance(it, end);
    std::size_t i = 0;

    for(; it != end; ++it, ++i) {
      ss << stringify(*it);
      if(i != size - 1)
        ss << delim_;
    }
    ss << end_;

    return ss.str();
  }
};

/// @brief Helper for `toString` methods converting `range`s
/// @{
template <class Range>
inline std::string toStringRange(Range&& range) {
  return RangeToString(",\n  ", "{\n  ", "\n}")(std::forward<Range>(range));
}
template <class Range, class StrinfigyFunctor>
inline std::string toStringRange(Range&& range, StrinfigyFunctor&& stringify) {
  return RangeToString(",\n  ", "{\n  ", "\n}")(std::forward<Range>(range), std::move(stringify));
}
/// @}

} // namespace core

} // namespace sequoia

#endif
