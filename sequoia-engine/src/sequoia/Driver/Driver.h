//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_DRIVER_DRIVER_H
#define SEQUOIA_DRIVER_DRIVER_H

#include "sequoia/Core/Platform.h"
#include "sequoia/Driver/Export.h"

namespace sequoia {

namespace driver {

/// @brief Initialize the engine and created the main Game object
/// @ingroup driver
class SEQUOIA_API Driver {
public:
  Driver() = delete;

#ifdef SEQUOIA_ON_WIN32
  static int run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#else
  static int run(int argc, char* argv[]);
#endif

private:
  static int runImpl();
};

} // namespace driver

using Driver = driver::Driver;

} // namespace sequoia

#endif
