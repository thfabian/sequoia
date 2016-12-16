//===-- sequoia/Core/String.cpp -----------------------------------------------------*- C++ -*-===//
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

#include "sequoia/Core/String.h"
#include <codecvt>
#include <cstring>
#include <cwchar>
#include <limits>
#include <locale>

#ifdef SEQUOIA_ON_WIN32
#include <Windows.h>
#include <crtdbg.h>
#endif

namespace sequoia {

namespace core {

static const char* copyCString(const char* str, std::size_t size) {
  char* buffer = new char[size + 1];
  std::memcpy(buffer, str, size + 1);
  return buffer;
}

static const wchar_t* copyCString(const wchar_t* str, std::size_t size) {
  wchar_t* buffer = new wchar_t[size + 1];
  std::wmemcpy(buffer, str, size + 1);
  return buffer;
}

const char* copyUtf8CString(const Utf8String& utf8) {
  return copyCString(utf8.c_str(), utf8.size());
}

const char* copyUtf8CString(const char* utf8) { return copyCString(utf8, std::strlen(utf8)); }

const wchar_t* copyUtf16CString(const Utf16String& utf16) {
  return copyCString(utf16.c_str(), utf16.size());
}

const wchar_t* copyUtf16CString(const wchar_t* utf16) {
  return copyCString(utf16, std::wcslen(utf16));
}

} // namespace core

} // namespace sequoia
