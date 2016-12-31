//===-- sequoia/Driver/Win32Console.cpp ---------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Driver/Win32Console.h"
#ifdef SEQUOIA_ON_WIN32

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cwchar>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace sequoia {

namespace driver {

/// @brief Return `true` if program is in its own console (cursor at 0,0) or false if it was
/// launched from an existing console
static bool separateConsole() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    return false;

  // if cursor position is (0,0) then we were launched in a separate console
  return ((!csbi.dwCursorPosition.X) && (!csbi.dwCursorPosition.Y));
}

bool Win32Console::pauseAfterFinish = true;

Win32Console::Win32Console() {
  if(AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
    SetConsoleTitle(L"Sequoia Debug Console");
    std::freopen("CONOUT$", "w", stdout);
    std::freopen("CONOUT$", "w", stderr);
  }
  bool inConsole = separateConsole();
  Win32Console::pauseAfterFinish = inConsole;

  std::cout << std::endl;
  std::ios_base::sync_with_stdio(true);
}

Win32Console::~Win32Console() {
  if(Win32Console::pauseAfterFinish)
    std::system("pause");
  FreeConsole();
}

} // namespace driver

} // namespace sequoia

#endif