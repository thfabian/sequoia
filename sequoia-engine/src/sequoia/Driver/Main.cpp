//===-- sequoia/Driver/Main.cpp -----------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Main entry point of the sequoia framework.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Driver/Driver.h"

#ifdef SEQUOIA_ON_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef SEQUOIA_ON_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  return Driver::run(hInstance, hPrevInstance, lpCmdLine, nCmdShow); // ???
}
#else
int main(int argc, char* argv[]) { return sequoia::Driver::run(argc, argv); }
#endif
