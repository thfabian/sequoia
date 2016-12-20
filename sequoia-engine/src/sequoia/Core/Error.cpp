//===-- sequoia/Core/Error.cpp ------------------------------------------------------*- C++ -*-===//
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

#include "sequoia/Core/Error.h"

#ifdef SEQUOIA_ON_WIN32
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#endif

#include <iostream>

namespace sequoia {

namespace core {

void Error::fatal(String message) noexcept {
#ifdef SEQUOIA_ON_WIN32
  MessageBoxA(NULL, message.c_str(), "Sequioa - Fatal Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
  std::cerr << "Sequoia: error: " << message.asUTF8_c_str() << std::endl;
#endif
  std::exit(EXIT_FAILURE);
}

} // namespace core

} // namespace sequoia
