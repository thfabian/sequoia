//===-- sequoia/Core/Error.h --------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// This file provides various error handling routines.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_ERROR_H
#define SEQUOIA_CORE_ERROR_H

#include "sequoia/Core/Core.h"
#include "sequoia/Core/String.h"

namespace sequoia {

namespace core {

/// @brief Various error handling routines.
/// 
/// @ingroup core
struct SEQUOIA_EXPORT Error {
  Error() = delete;
  
  /// @brief Report a fatal error and exit with `EXIT_FAILURE`
  /// 
  /// This will print the message to `stderr` on Unix systems or open a `MessageBoxA` on Win32.
  /// 
  /// @param message  Message to display
  SEQUOIA_ATTRIBUTE_NORETURN static void fatal(String message) noexcept;
};


} // namespace core

} // namespace sequoia

#endif
