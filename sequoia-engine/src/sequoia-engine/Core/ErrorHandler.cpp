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

#include "sequoia-engine/Core/ErrorHandler.h"
#include "sequoia-engine/Core/Platform.h"
#include <cstdlib>
#include <iostream>

namespace sequoia {

namespace core {

static void defaultFatalErrorHandler(std::string message, bool crash) {
  std::cerr << "error: " << message << std::endl;
  if(crash)
    std::abort();
  else
    std::exit(EXIT_FAILURE);
}

static void defaultFatalErrorHandlerW(std::wstring message, bool crash) {
  std::wcerr << L"error: " << message << std::endl;
  if(crash)
    std::abort();
  else
    std::exit(EXIT_FAILURE);
}

static ErrorHandler::FatalErrorHandler fatalErrorHandler = &defaultFatalErrorHandler;
static ErrorHandler::FatalErrorHandlerW fatalErrorHandlerW = &defaultFatalErrorHandlerW;

void ErrorHandler::fatal(std::string message, bool crash) { fatalErrorHandler(message, crash); }

void ErrorHandler::fatal(std::wstring message, bool crash) { fatalErrorHandlerW(message, crash); }

void ErrorHandler::setFatalErrorHandler(ErrorHandler::FatalErrorHandler handler) {
  fatalErrorHandler = handler ? handler : &defaultFatalErrorHandler;
}

void ErrorHandler::setFatalErrorHandler(ErrorHandler::FatalErrorHandlerW handler) {
  fatalErrorHandlerW = handler ? handler : &defaultFatalErrorHandlerW;
}

} // namespace core

} // namespace sequoia
