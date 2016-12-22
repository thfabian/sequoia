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
#include <cwchar>
#include <iosfwd>
#include <iterator>
#include <string>

#include <OGRE/OgreUTFString.h>

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

/// @brief An UTF-16 string with implicit conversion to/from `std::string` and `std::wstring`
using String = Ogre::UTFString;

/// @brief The default string type for the Platform (`std::string` for Unix and `std::wstring` for
/// Win32)
#ifdef SEQUOIA_ON_WIN32
using DefaultString = std::wstring;
#else
using DefaultString = std::string;
#endif

/// @brief Return a copy of the string `s`
///
/// @throw std::bad_alloc  Out of memory
/// @{
SEQUOIA_EXPORT extern const char* copyCString(const std::string& s);
SEQUOIA_EXPORT extern const char* copyCString(const char* s);
SEQUOIA_EXPORT extern const wchar_t* copyCString(const std::wstring& s);
SEQUOIA_EXPORT extern const wchar_t* copyCString(const wchar_t* s);
/// @}

/// @macro CSTR
/// @brief Character string (L"" on Win32 or "" on Unix)
///
/// @code
///   CSTR("hello world") // -> "hello world" on Unix or L"hello world" on Win32
/// @endcode
#ifdef CSTR
#error "CSTR already defined!"
#endif
#ifdef SEQUOIA_ON_WIN32
#define CSTR(x) Lx
#else
#define CSTR(x) x
#endif

/// @}

} // namespace core

using String = core::String;
using DefaultString = core::DefaultString;

} // namespace sequoia

#endif
