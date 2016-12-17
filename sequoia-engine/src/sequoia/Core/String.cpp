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
#include <cstring>
#include <cwchar>

namespace sequoia {

namespace core {

static const char* copyCStringImpl(const char* str, const std::size_t size) {
  char* buffer = new char[size + 1];
  std::memcpy(buffer, str, size + 1);
  return buffer;
}

static const wchar_t* copyCStringImpl(const wchar_t* str, const std::size_t size) {
  wchar_t* buffer = new wchar_t[size + 1];
  std::wmemcpy(buffer, str, size + 1);
  return buffer;
}

const char* copyCString(const std::string& s) { return copyCStringImpl(s.c_str(), s.size()); }
const char* copyCString(const char* s) { return copyCStringImpl(s, std::strlen(s)); }
const wchar_t* copyCString(const std::wstring& s) { return copyCStringImpl(s.c_str(), s.size()); }
const wchar_t* copyCString(const wchar_t* s) { return copyCStringImpl(s, std::wcslen(s)); }

} // namespace core

} // namespace sequoia
