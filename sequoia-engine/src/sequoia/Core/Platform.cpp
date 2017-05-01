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

#include "sequoia/Core/Platform.h"
#include <cstring>

namespace sequoia {

namespace platform {

static char* copyCStringImpl(const char* str, const std::size_t size) {
  char* buffer = new char[size + 1];
  std::memcpy(buffer, str, size + 1);
  return buffer;
}

static wchar_t* copyCStringImpl(const wchar_t* str, const std::size_t size) {
  wchar_t* buffer = new wchar_t[size + 1];
  std::wmemcpy(buffer, str, size + 1);
  return buffer;
}

char* copyCString(const std::string& str) { return copyCStringImpl(str.c_str(), str.size()); }
char* copyCString(const char* str) { return copyCStringImpl(str, std::strlen(str)); }
wchar_t* copyCString(const std::wstring& str) { return copyCStringImpl(str.c_str(), str.size()); }
wchar_t* copyCString(const wchar_t* str) { return copyCStringImpl(str, std::wcslen(str)); }

} // namespace platform

} // namespace sequoia
