//===-- sequoia/Core/Exception.cpp --------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Exception classes of sequoia.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Exception.h"
#include <iostream>
#include <memory>
#include <sstream>

namespace sequoia {

namespace core {

//===--------------------------------------------------------------------------------------------===
//  Exception
//===--------------------------------------------------------------------------------------------===

template <bool CharacterTypeIsWide>
struct ToStream {
  template <class StreamType>
  static void apply(StreamType& stream, const Exception& exception) {
    if(exception.path())
      stream << exception.path() << ":" << exception.line() << ": ";
    stream << exception.message().toAnsiString();
  }
};

template <>
struct ToStream<true> {
  template <class StreamType>
  static void apply(StreamType& stream, const Exception& exception) {
    if(exception.path())
      stream << exception.path() << ":" << exception.line() << ": ";
    stream << exception.message().toWideString();
  }
};

Exception::Exception(const std::string& whatArg, int line, const char* path)
    : message_(whatArg), line_(line), path_(path) {}

Exception::Exception(const std::wstring& whatArg, int line, const char* path)
    : message_(whatArg), line_(line), path_(path) {}

Exception::~Exception() noexcept {}

const char* Exception::what() const noexcept {
#ifndef NDEBUG
  std::stringstream ss;
  ToStream<false>::apply(ss, *this);
  std::string whatStr = ss.str();
#else
  std::string whatStr = message_.toAnsiString();
#endif
  return copyCString(whatStr);
}

int Exception::line() const noexcept { return line_; }

const char* Exception::path() const noexcept { return path_; }

const String& Exception::message() const noexcept { return message_; }

std::ostream& operator<<(std::ostream& stream, const Exception& exception) {
  ToStream<false>::apply(stream, exception);
  return stream;
}

std::wostream& operator<<(std::wostream& stream, const Exception& exception) {
  ToStream<true>::apply(stream, exception);
  return stream;
}

} // namespace core

} // namespace sequoia
