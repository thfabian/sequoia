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

#include "sequoia/Driver/Win32Console.h"
#ifdef SEQUOIA_ON_WIN32

#include "sequoia/Core/Platform.h"
#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <iostream>

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
