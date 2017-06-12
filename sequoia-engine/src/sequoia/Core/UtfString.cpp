//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/UtfString.h"
#include <cstring>
#include <iterator>

namespace sequoia {

namespace core {

const std::size_t UtfString::InvalidPos = std::basic_string<std::uint32_t>::npos;

UtfString::UtfString() {}

UtfString::UtfString(char ansiChar, const std::locale& locale) {
  string_ += Utf32::decodeAnsi(ansiChar, locale);
}

UtfString::UtfString(wchar_t wideChar) { string_ += Utf32::decodeWide(wideChar); }

UtfString::UtfString(std::uint32_t utf32Char) { string_ += utf32Char; }

UtfString::UtfString(const char* ansiString, const std::locale& locale) {
  if(ansiString) {
    std::size_t length = strlen(ansiString);
    if(length > 0) {
      string_.reserve(length + 1);
      Utf32::fromAnsi(ansiString, ansiString + length, std::back_inserter(string_), locale);
    }
  }
}

UtfString::UtfString(const std::string& ansiString, const std::locale& locale) {
  string_.reserve(ansiString.length() + 1);
  Utf32::fromAnsi(ansiString.begin(), ansiString.end(), std::back_inserter(string_), locale);
}

UtfString::UtfString(const wchar_t* wideString) {
  if(wideString) {
    std::size_t length = std::wcslen(wideString);
    if(length > 0) {
      string_.reserve(length + 1);
      Utf32::fromWide(wideString, wideString + length, std::back_inserter(string_));
    }
  }
}

UtfString::UtfString(const std::wstring& wideString) {
  string_.reserve(wideString.length() + 1);
  Utf32::fromWide(wideString.begin(), wideString.end(), std::back_inserter(string_));
}

UtfString::UtfString(const std::uint32_t* utf32String) {
  if(utf32String)
    string_ = utf32String;
}

UtfString::UtfString(const std::basic_string<std::uint32_t>& utf32String) : string_(utf32String) {}

UtfString::UtfString(const UtfString& copy) : string_(copy.string_) {}

UtfString::operator std::string() const { return toAnsiString(); }

UtfString::operator std::wstring() const { return toWideString(); }

std::string UtfString::toAnsiString(const std::locale& locale) const {
  // Prepare the output string
  std::string output;
  output.reserve(string_.length() + 1);

  // Convert
  Utf32::toAnsi(string_.begin(), string_.end(), std::back_inserter(output), 0, locale);

  return output;
}

std::wstring UtfString::toWideString() const {
  // Prepare the output string
  std::wstring output;
  output.reserve(string_.length() + 1);

  // Convert
  Utf32::toWide(string_.begin(), string_.end(), std::back_inserter(output), 0);

  return output;
}

std::basic_string<std::uint8_t> UtfString::toUtf8() const {
  // Prepare the output string
  std::basic_string<std::uint8_t> output;
  output.reserve(string_.length());

  // Convert
  Utf32::toUtf8(string_.begin(), string_.end(), std::back_inserter(output));

  return output;
}

std::basic_string<std::uint16_t> UtfString::toUtf16() const {
  // Prepare the output string
  std::basic_string<std::uint16_t> output;
  output.reserve(string_.length());

  // Convert
  Utf32::toUtf16(string_.begin(), string_.end(), std::back_inserter(output));

  return output;
}

std::basic_string<std::uint32_t> UtfString::toUtf32() const { return string_; }

UtfString& UtfString::operator=(const UtfString& right) {
  string_ = right.string_;
  return *this;
}

UtfString& UtfString::operator+=(const UtfString& right) {
  string_ += right.string_;
  return *this;
}

std::uint32_t UtfString::operator[](std::size_t index) const { return string_[index]; }

std::uint32_t& UtfString::operator[](std::size_t index) { return string_[index]; }

void UtfString::clear() { string_.clear(); }

std::size_t UtfString::getSize() const { return string_.size(); }

bool UtfString::empty() const { return string_.empty(); }

void UtfString::erase(std::size_t position, std::size_t count) { string_.erase(position, count); }

void UtfString::insert(std::size_t position, const UtfString& str) {
  string_.insert(position, str.string_);
}

std::size_t UtfString::find(const UtfString& str, std::size_t start) const {
  return string_.find(str.string_, start);
}

void UtfString::replace(std::size_t position, std::size_t length, const UtfString& replaceWith) {
  string_.replace(position, length, replaceWith.string_);
}

void UtfString::replace(const UtfString& searchFor, const UtfString& replaceWith) {
  std::size_t step = replaceWith.getSize();
  std::size_t len = searchFor.getSize();
  std::size_t pos = find(searchFor);

  // Replace each occurrence of search
  while(pos != InvalidPos) {
    replace(pos, len, replaceWith);
    pos = find(searchFor, pos + step);
  }
}

UtfString UtfString::substr(std::size_t position, std::size_t length) const {
  return string_.substr(position, length);
}

const std::uint32_t* UtfString::data() const { return string_.c_str(); }

UtfString::Iterator UtfString::begin() { return string_.begin(); }

UtfString::ConstIterator UtfString::begin() const { return string_.begin(); }

UtfString::Iterator UtfString::end() { return string_.end(); }

UtfString::ConstIterator UtfString::end() const { return string_.end(); }

bool operator==(const UtfString& left, const UtfString& right) {
  return left.string_ == right.string_;
}

bool operator!=(const UtfString& left, const UtfString& right) { return !(left == right); }

bool operator<(const UtfString& left, const UtfString& right) {
  return left.string_ < right.string_;
}

bool operator>(const UtfString& left, const UtfString& right) { return right < left; }

bool operator<=(const UtfString& left, const UtfString& right) { return !(right < left); }

bool operator>=(const UtfString& left, const UtfString& right) { return !(left < right); }

UtfString operator+(const UtfString& left, const UtfString& right) {
  UtfString string = left;
  string += right;

  return string;
}

} // namespace core

} // namespace sequoia
