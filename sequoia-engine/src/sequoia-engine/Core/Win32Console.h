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

#ifndef SEQUOIA_ENGINE_CORE_WIN32CONSOLE_H
#define SEQUOIA_ENGINE_CORE_WIN32CONSOLE_H

#include "sequoia-engine/Core/Export.h"

#ifdef SEQUOIA_ON_WIN32

namespace sequoia {

namespace core {

/// @brief Attach a Win32 Console for debugging purposes
/// @ingroup core
class SEQUOIA_API Win32Console {
public:
  /// @brief Attach a Win32 console to the current process
  Win32Console();

  /// @brief Remove console
  ~Win32Console();

  /// @brief Pause after application has finished (default: true)
  static bool pauseAfterFinish;
};

} // namespace core

using Win32Console = core::Win32Console;

} // namespace sequoia

#endif

#endif
