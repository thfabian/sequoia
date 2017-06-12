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

#include "sequoia/Driver/Export.h"

#ifdef SEQUOIA_ON_WIN32

namespace sequoia {

namespace driver {

/// @brief Attach a Win32 Console for debugging purposes
/// @ingroup driver
class SEQUOIA_DRIVER_API Win32Console {
public:
  /// @brief Attach a Win32 console to the current process
  Win32Console();

  /// @brief Remove console
  ~Win32Console();

  /// @brief Pause after application has finished (default: true)
  static bool pauseAfterFinish;
};

} // namespace driver

using Win32Console = driver::Win32Console;

} // namespace sequoia

#endif
