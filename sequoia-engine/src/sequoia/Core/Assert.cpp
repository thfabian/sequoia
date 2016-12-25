//===-- sequoia/Core/Assert.cpp -----------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// This file provides various error handling routines.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Core.h"
#include "sequoia/Core/ErrorHandler.h"
#include <sstream>

namespace sequoia {

namespace core {

static void assertionFailedImpl(char const* expr, char const* msg, char const* function,
                                char const* file, long line) {
  std::stringstream ss;
  ss << "Assertion failed: `" << expr << "' " << (msg == nullptr ? "" : msg) << "\n"
     << "Function:\n" << function << "'"
     << "\n"
     << "Location:\n" << file << ":" << line << "\n";
  ErrorHandler::getSingleton().fatal(ss.str());
}

void assertionFailed(char const* expr, char const* function, char const* file, long line) {
  assertionFailedImpl(expr, nullptr, function, file, line);
}

void assertionFailedMsg(char const* expr, char const* msg, char const* function, char const* file,
                        long line) {
  assertionFailedImpl(expr, msg, function, file, line);
}

} // namespace core

} // namespace sequoia