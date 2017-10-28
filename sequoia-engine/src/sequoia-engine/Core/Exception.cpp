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

#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Platform.h"
#include <iostream>
#include <memory>
#include <sstream>

namespace sequoia {

namespace core {

namespace {

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
      stream << UtfString(exception.path()).toWideString() << L":" << exception.line() << L": ";
    stream << exception.message().toWideString();
  }
};

} // anonymous namespace

Exception::~Exception() noexcept {}

const char* Exception::what() const noexcept {
#ifndef NDEBUG
  std::stringstream ss;
  ToStream<false>::apply(ss, *this);
  return platform::copyCString(ss.str().c_str());
#else
  return platform::copyCString(message_.toAnsiString().c_str());
#endif
}

int Exception::line() const noexcept { return line_; }

const char* Exception::path() const noexcept { return path_; }

const UtfString& Exception::message() const noexcept { return message_; }

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
