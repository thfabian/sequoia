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

#include "sequoia/Core/Core.h"
#include "sequoia/Core/Error.h"
#include "sequoia/Driver/Driver.h"

#ifdef SEQUOIA_ON_WIN32
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#endif

#ifdef SEQUOIA_ON_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char** argv)
#endif
{

  sequoia::core::Error::fatal("oops, that didn't work out so well!");

  return 0;
}
