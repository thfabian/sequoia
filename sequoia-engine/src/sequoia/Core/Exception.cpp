#include "sequoia/Core/Exception.h"
#include <iostream>
#include <memory>
#include <sstream>

namespace sequoia {

namespace core {

//===--------------------------------------------------------------------------------------------===
//  Exception
//===--------------------------------------------------------------------------------------------===

template <class StreamType>
static void toStream(StreamType& stream, const Exception& exception) {
  if(exception.path())
    stream << exception.path() << ":" << exception.line() << ": ";
  stream << exception.what();
}

Exception::Exception(const Utf8String& whatArg, int line, const char* path)
    : whatArg_(whatArg), line_(line), path_(path) {}

Exception::Exception(const Utf16String& whatArg, int line, const char* path)
    : whatArg_(Utf16ToUtf8(whatArg)), line_(line), path_(path) {}

Exception::~Exception() noexcept {}

const char* Exception::what() const noexcept {
#ifndef NDEBUG
  std::stringstream ss;
  toStream(ss, *this);
  return copyUtf8CString(ss.str().c_str());
#else
  return whatArg_.c_str();
#endif
}

int Exception::line() const noexcept { return line_; }

const char* Exception::path() const noexcept { return path_; }

std::ostream& operator<<(std::ostream& stream, const Exception& exception) {
  toStream(stream, exception);
  return stream;
}

std::wostream& operator<<(std::wostream& stream, const Exception& exception) {
  toStream(stream, exception);
  return stream;
}

} // namespace core

} // namespace sequoia
