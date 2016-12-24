//===-- sequoia/Driver/Driver.h -----------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Setup main-loop of the framework.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_DRIVER_DRIVER_H
#define SEQUOIA_DRIVER_DRIVER_H

#include "sequoia/Core/Core.h"

#ifdef SEQUOIA_ON_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace sequoia {

namespace driver {

/// @brief Initialize framework and invoke main-loop
///
/// @ingroup driver
class SEQUOIA_EXPORT Driver {
public:
  Driver() = delete;

#ifdef SEQUOIA_ON_WIN32
  static int run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#else
  static int run(int argc, char* argv[]);
#endif

private:
  static void setDefaultConfigs();
  static int runImpl();
};

} // namespace driver

using Driver = driver::Driver;

} // namespace sequoia

#endif
