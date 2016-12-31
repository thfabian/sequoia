//===-- sequoia/Driver/Win32Console.h -----------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Driver/Export.h"

#ifdef SEQUOIA_ON_WIN32

namespace sequoia {

namespace driver {

/// @brief  Attach a Win32 Console for debugging purpose
///
/// @ingroup driver
class SEQUOIA_DRIVER_EXPORT Win32Console {
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