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

#include "sequoia/Driver/Driver.h"
#include "sequoia/Driver/Win32Console.h"

#ifdef SEQUOIA_ON_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef SEQUOIA_ON_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  sequoia::Win32Console win32Console;
  return sequoia::Driver::run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
#else
int main(int argc, char* argv[]) { return sequoia::Driver::run(argc, argv); }
#endif
