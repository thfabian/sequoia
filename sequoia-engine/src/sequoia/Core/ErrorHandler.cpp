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

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Platform.h"
#include <cstdlib>
#include <iostream>

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(core::ErrorHandler);

namespace core {

ErrorHandler::ErrorHandler(UtfString program) {
  platform::String programStr = program;
  platform::Path p(programStr);
  program_ = p.stem().native();
}

void ErrorHandler::fatal(std::string message, bool messagebox, bool crash) noexcept {
  (void)messagebox;
#ifdef SEQUOIA_ON_WIN32
  if(messagebox) {
    MessageBoxW(NULL, UtfString(message).toWideString().c_str(), L"Fatal Error",
                MB_OK | MB_ICONERROR | MB_TASKMODAL);
  } else
    std::cerr << program_.toAnsiString() << ": error: " << message << std::endl;
#else
  std::cerr << program_.toAnsiString() << ": error: " << message << std::endl;
#endif

  if(crash)
    std::abort();
  else
    std::exit(EXIT_FAILURE);
}

void ErrorHandler::fatal(std::wstring message, bool messagebox, bool crash) noexcept {
  (void)messagebox;
#ifdef SEQUOIA_ON_WIN32
  if(messagebox)
    MessageBoxW(NULL, message.c_str(), L"Fatal Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
  else
    std::wcerr << program_.toWideString() << L": error: " << message << std::endl;
#else
  std::wcerr << program_.toWideString() << L": error: " << message << std::endl;
#endif

  if(crash)
    std::abort();
  else
    std::exit(EXIT_FAILURE);
}

void ErrorHandler::warning(std::string message) noexcept {
  std::cerr << program_.toAnsiString() << ": warning: " << message << std::endl;
}

void ErrorHandler::warning(std::wstring message) noexcept {
  std::wcerr << program_.toWideString() << L": warning: " << message << std::endl;
}

UtfString ErrorHandler::program() const noexcept { return program_; }

} // namespace core

} // namespace sequoia
