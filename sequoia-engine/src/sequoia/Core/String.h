//===-- sequoia/Core/String.h -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Functions and typedefs to manipulate strings.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_STRING
#define SEQUOIA_CORE_STRING

#include "sequoia/Core/Core.h"
#include <SFML/System/String.hpp>
#include <iosfwd>
#include <string>

namespace sequoia {

/// @addtogroup core
/// @{

namespace core {

/// @typedef String
/// @brief Utility string class that automatically handles conversions between types and encodings
///
/// sequoia::core::String is a utility string class defined mainly for convenience. It is a Unicode
/// string (implemented using UTF-32), thus it can store any character in the world (European,
/// Chinese, Arabic, Hebrew, etc.).
///
/// It automatically handles conversions from/to ANSI and wide strings, so that you can work with
/// standard string classes and still be compatible with functions taking a sf::String.
///
/// @code
///   sequoia::core::String s;
///
///   std::string s1 = s;  // automatically converted to ANSI string
///   std::wstring s2 = s; // automatically converted to wide string
///   s = "hello";         // automatically converted from ANSI string
///   s = L"hello";        // automatically converted from wide string
///   s += 'a';            // automatically converted from ANSI string
///   s += L'a';           // automatically converted from wide string
/// @endcode
///
/// Conversions involving ANSI strings use the default user locale. However it is possible to use a
/// custom locale if necessary:
///
/// @code
///   std::locale locale;
///   sf::String s;
///   // ...
///   std::string s1 = s.toAnsiString(locale);
///   s = sequoia::core::String("hello", locale);
/// @endcode
///
/// sequoia::core::String defines the most important functions of the standard std::string class:
/// removing, random access, iterating, appending, comparing, etc. However it is a simple class
/// provided for convenience, and you may have to consider using a more optimized class if your
/// program requires complex string handling.
///
using String = sf::String;

/// @brief Return a copy of the string `s`
///
/// @throw std::bad_alloc  Out of memory
/// @{
SEQUOIA_EXPORT extern const char* copyCString(const std::string& s);
SEQUOIA_EXPORT extern const char* copyCString(const char* s);
SEQUOIA_EXPORT extern const wchar_t* copyCString(const std::wstring& s);
SEQUOIA_EXPORT extern const wchar_t* copyCString(const wchar_t* s);
/// @}

} // namespace core

/// @}

using String = core::String;

} // namespace sequoia

#endif
