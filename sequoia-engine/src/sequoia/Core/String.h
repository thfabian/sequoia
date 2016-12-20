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

#ifndef SEQUOIA_CORE_STRING_H
#define SEQUOIA_CORE_STRING_H

#include "sequoia/Core/Core.h"
#include <iosfwd>
#include <cwchar>
#include <string>
#include <iterator>

#include <OGRE/OgreUTFString.h>

namespace sequoia {

namespace core {

/// @brief An UTF-16 string with implicit conversion to/from `std::string` and `std::wstring`
using String = Ogre::UTFString;

/// @brief Return a copy of the string `s`
///
/// @throw std::bad_alloc  Out of memory
/// @{
SEQUOIA_EXPORT extern const char* copyCString(const std::string& s);
SEQUOIA_EXPORT extern const char* copyCString(const char* s);
SEQUOIA_EXPORT extern const wchar_t* copyCString(const std::wstring& s);
SEQUOIA_EXPORT extern const wchar_t* copyCString(const wchar_t* s);
/// @}

/// @}

} // namespace core

using String = core::String;

} // namespace sequoia

#endif
