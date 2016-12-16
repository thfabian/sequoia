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
#include <iosfwd>
#include <string>

namespace sequoia {

/// @addtogroup core
/// @{

namespace core {

/// @typedef String
/// @brief Unicode aware string type
using String = sf::String;

/// @brief Copy C String
///
/// @throw std::bad_alloc  Out of memory
/// @{
SEQUOIA_EXPORT extern const char* copyCString(const Utf8String& utf8);
SEQUOIA_EXPORT extern const char* copyCString(const char* utf8);
SEQUOIA_EXPORT extern const wchar_t* copyCString(const Utf16String& utf8);
SEQUOIA_EXPORT extern const wchar_t* copyCString(const wchar_t* utf8);
/// @}

} // namespace core

/// @}

using String = core::String;

} // namespace sequoia

#endif
