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

#ifndef SEQUOIA_CORE_PLATFORM_H
#define SEQUOIA_CORE_PLATFORM_H

#include "sequoia/Core/Compiler.h"
#include "sequoia/Core/Export.h"
#include <boost/filesystem.hpp>
#include <cwchar>
#include <string>

#ifdef SEQUOIA_ON_WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef APIENTRY
#undef APIENTRY
#endif

#include <Windows.h>
#endif

namespace sequoia {

/// @addtogroup core
/// @{

namespace platform {

namespace filesystem = boost::filesystem;

/// @typedef Path
/// @brief The native path of the platform with respect to `platform::String`
using Path = filesystem::path;

/// @brief Convert `path` to `platform::Path`
/// @param path   Path to convert
/// @{
SEQUOIA_CORE_API extern platform::Path asPath(const char* path);
SEQUOIA_CORE_API extern platform::Path asPath(const wchar_t* path);
SEQUOIA_CORE_API extern platform::Path asPath(const std::string& path);
SEQUOIA_CORE_API extern platform::Path asPath(const std::wstring& path);
/// @}

/// @brief Convert `path` to `std::string`
/// @param path   Path to convert
SEQUOIA_CORE_API extern std::string toAnsiString(const platform::Path& path);

/// @typedef String
/// @brief The native string of the platform (`std::string` for Unix and `std::wstring` for Win32)
#ifdef SEQUOIA_ON_WIN32
using String = std::wstring;
#else
using String = std::string;
#endif

/// @typedef Char
/// @brief The native character type of the platform (`char` for Unix and `wchar_t` for Win32)
using Char = String::value_type;

/// @macro PLATFORM_STR
/// @brief Native character string (`L""` on Win32 or `""` on Unix)
///
/// @code{.cpp}
///   PLATFORM_STR("hello world") // -> "hello world" on Unix or L"hello world" on Win32
/// @endcode
#ifdef PLATFORM_STR
#error "PLATFORM_STR already defined!"
#endif
#ifdef SEQUOIA_ON_WIN32
#define PLATFORM_STR_IMPL(x) L##x
#define PLATFORM_STR(x) PLATFORM_STR_IMPL(x)
#else
#define PLATFORM_STR(x) x
#endif

/// @brief Return a copy of the string `str`
/// @throw std::bad_alloc  Out of memory
/// @{
SEQUOIA_CORE_API extern char* copyCString(const std::string& str);
SEQUOIA_CORE_API extern char* copyCString(const char* str);
SEQUOIA_CORE_API extern wchar_t* copyCString(const std::wstring& wstr);
SEQUOIA_CORE_API extern wchar_t* copyCString(const wchar_t* wstr);
/// @}

/// @}

} // namespace platform

} // namespace sequoia

#endif
