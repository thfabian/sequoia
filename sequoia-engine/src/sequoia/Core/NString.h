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
#include <string>
#include <memory>

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

/// @brief The nativ string for the platform (`std::string` for Unix and `std::wstring` for Win32)
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
SEQUOIA_CORE_EXPORT extern std::unqiue_ptr<char[]> copyCString(const std::string& str);
SEQUOIA_CORE_EXPORT extern std::unqiue_ptr<char[]> copyCString(const char* str);
SEQUOIA_CORE_EXPORT extern std::unqiue_ptr<wchar_t[]> copyCString(const std::wstring& wstr);
SEQUOIA_CORE_EXPORT extern std::unqiue_ptr<wchar_t[]> copyCString(const wchar_t* wstr);
/// @}

/// @brief Conversion between `char` and `wchar_t` strings
///
/// These versions will consume the input string.
///
/// @throw std::invalid_argument  Invalid string
/// @{
SEQUOIA_CORE_EXPORT extern std::string toString(std::string& str);
SEQUOIA_CORE_EXPORT extern std::string toString(std::wstring& wstr);
SEQUOIA_CORE_EXPORT extern std::wstring toWString(std::string& str);
SEQUOIA_CORE_EXPORT extern std::wstring toWString(std::wstring& wstr);
/// @}

/// @brief Conversion between `char` and `wchar_t` strings (no throw)
///
/// These versions will always allocate a new string (even if no conversion is performed).
///
/// @param in    Input string to convert
/// @param out   Output string which on success contains the converted data, otherwise unmodified
/// @returns `true` on success, `false` otherwise
/// @{
SEQUOIA_CORE_EXPORT extern bool toString(const std::string& in, std::string& out) noexcept;
SEQUOIA_CORE_EXPORT extern bool toString(const std::wstring& in, std::string& out) noexcept;
SEQUOIA_CORE_EXPORT extern bool toWString(const std::string& in, std::string& out) noexcept;
SEQUOIA_CORE_EXPORT extern bool toWString(const std::wstring& in, std::string& out) noexcept;
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
