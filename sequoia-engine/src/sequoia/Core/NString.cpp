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

#include "sequoia/Core/NString.h"
#include <cstring>

namespace sequoia {

namespace core {

static std::unique_ptr<char[]> copyCStringImpl(const char* str, const std::size_t size) {
  std::unique_ptr<char[]> buffer(new char[size + 1]);
  std::memcpy(buffer.get(), str, size + 1);
  return buffer;
}

static std::unique_ptr<wchar_t[]> copyCStringImpl(const wchar_t* str, const std::size_t size) {
  std::unique_ptr<wchar_t[]> buffer(new wchar_t[size + 1]);
  std::wmemcpy(buffer.get(), str, size + 1);
  return buffer;
}

std::unique_ptr<char[]> copyCString(const std::string& str) { return copyCStringImpl(str.c_str(), str.size()); }
std::unique_ptr<char[]> copyCString(const char* str) { return copyCStringImpl(str, std::strlen(str)); }
std::unique_ptr<wchar_t[]> copyCString(const std::wstring& str) { return copyCStringImpl(str.c_str(), str.size()); }
std::unique_ptr<wchar_t[]> copyCString(const wchar_t* str) { return copyCStringImpl(str, std::wcslen(str)); }

} // namespace core

} // namespace sequoia
