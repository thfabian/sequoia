//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_NSTRING_H
#define SEQUOIA_CORE_NSTRING_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Platform.h"
#include <cwchar>
#include <iosfwd>
#include <memory>
#include <string>

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

/// @brief The native string of the platform (`std::string` for Unix and `std::wstring` for Win32)
#ifdef SEQUOIA_ON_WIN32
using NString = std::wstring;
#else
using NString = std::string;
#endif

/// @brief The native character type for the platform (`char` for Unix and `wchar_t` for Win32)
using NChar = NString::value_type;

/// @brief Return a copy of the string `str`
/// @throw std::bad_alloc  Out of memory
/// @{
SEQUOIA_CORE_API extern std::unique_ptr<char[]> copyCString(const std::string& str);
SEQUOIA_CORE_API extern std::unique_ptr<char[]> copyCString(const char* str);
SEQUOIA_CORE_API extern std::unique_ptr<wchar_t[]> copyCString(const std::wstring& wstr);
SEQUOIA_CORE_API extern std::unique_ptr<wchar_t[]> copyCString(const wchar_t* wstr);
/// @}

/// @macro CSTR
/// @brief Native character string (`L""` on Win32 or `""` on Unix)
///
/// @code
///   NSTR("hello world") // -> "hello world" on Unix or L"hello world" on Win32
/// @endcode
#ifdef NSTR
#error "NSTR already defined!"
#endif
#ifdef SEQUOIA_ON_WIN32
#define NSTR_IMPL(x) L##x
#define NSTR(x) NSTR_IMPL(x)
#else
#define NSTR(x) x
#endif

/// @}

} // namespace core

using NString = core::NString;
using NChar = core::NChar;

} // namespace sequoia

#endif
