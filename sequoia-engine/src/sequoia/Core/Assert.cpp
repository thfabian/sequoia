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

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/NString.h"
#include <sstream>

namespace sequoia {

namespace core {

static void assertionFailedImpl(char const* expr, char const* msg, char const* function,
                                char const* file, long line) {
  std::stringstream ss;
  ss << "Assertion failed: `" << expr << "' " << (msg == nullptr ? "" : msg) << "\n"
     << "Function:\n" << function << "'\n"
     << "Location:\n" << file << ":" << line << "\n";
  ErrorHandler::getSingleton().fatal(ss.str());
}

static void assertionFailedImpl(char const* expr, char wchar_t* msg, char const* function,
                                char const* file, long line) {
  std::wstringstream ss;
  ss << L"Assertion failed: `" << toWString(expr) << "' " << (msg == nullptr ? "" : msg) << "\n"
     << L"Function:\n" << toWString(function) << L"'\n"
     << L"Location:\n" << toWString(file) << L":" << line << L"\n";
  ErrorHandler::getSingleton().fatal(ss.str());
}

void assertionFailedMsg(const char* expr, const char* msg, const char* function, const char* file, 
                     long line) {
  assertionFailedImpl(expr, msg, function, file, line);
}

void assertionFailedMsg(char const* expr, char wchar_t* msg, char const* function, char const* file,
                     long line) {
  assertionFailedImpl(expr, msg, function, file, line);
}

void assertionFailed(char const* expr,  char const* function, const char* file, long line) {
  assertionFailedImpl(expr, (const char*) NULL, function, file, line);
}

} // namespace core

} // namespace sequoia

