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
#include "sequoia/Core/NPath.h"
#include <iostream>
#include <cstdlib>

namespace sequoia {

namespace core {

ErrorHandler::ErrorHandler(NString program) {
  NPath p(program);
  program_ = p.stem().native();
}

void ErrorHandler::fatal(std::string message, bool messagebox) noexcept {
#ifdef SEQUOIA_ON_WIN32
  if(messagebox)
    MessageBoxW(NULL, toWString(message).c_str(), L"Fatal Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
  else
    std::err << toString(program_) << ": error: " << message << std::endl;
#else
  std::err << toString(program_) << ": error: " << message << std::endl;
#endif
  std::abort();
}

void ErrorHandler::fatal(std::wstring message, bool messagebox) noexcept {
#ifdef SEQUOIA_ON_WIN32
  if(messagebox)
    MessageBoxW(NULL, message.c_str(), L"Fatal Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
  else
    std::werr << toWString(program_) << L": error: " << message << std::endl;
#else
  std::werr << toWString(program_) << L": error: " << message << std::endl;
#endif
  std::abort();
}

void ErrorHandler::warning(std::string message) noexcept {
  std::cerr << toString(program_) << ": warning: " << message << std::endl;
}

void ErrorHandler::warning(std::wstring message) noexcept {
  std::wcerr << toWString(program_) << L": warning: " << message << std::endl;
}

NString ErrorHandler::program() const noexcept { return program_; }

} // namespace core

} // namespace sequoia
