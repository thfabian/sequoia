//===-- sequoia/Core/ErrorHandler.cpp -----------------------------------------------*- C++ -*-===//
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

#include "sequoia/Core/ErrorHandler.h"
#include <boost/filesystem.hpp>

#ifdef SEQUOIA_ON_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <iostream>

template <>
sequoia::core::ErrorHandler* Ogre::Singleton<sequoia::core::ErrorHandler>::msSingleton = nullptr;

namespace sequoia {

namespace core {

ErrorHandler::ErrorHandler(String program) {
#ifdef SEQUOIA_ON_WIN32
  boost::filesystem::path p(program.asWStr());
#else
  boost::filesystem::path p(program.asUTF8());
#endif
  program_ = p.stem().string();
}

void ErrorHandler::fatal(String message, bool messagebox) noexcept {
#ifdef SEQUOIA_ON_WIN32
  if(messagebox)
    MessageBoxW(NULL, message.asWStr_c_str(), L"Fatal Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
  else
    std::wcerr << program_.asWStr() << L": error: " << message.asWStr() << std::endl;
#else
  std::cerr << program_.asUTF8() << ": error: " << message.asUTF8() << std::endl;
#endif
  std::exit(EXIT_FAILURE);
}

String ErrorHandler::program() const noexcept { return program_; }

} // namespace core

} // namespace sequoia
