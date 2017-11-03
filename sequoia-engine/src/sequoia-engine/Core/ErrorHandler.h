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

#ifndef SEQUOIA_ENGINE_CORE_ERRORHANDLER_H
#define SEQUOIA_ENGINE_CORE_ERRORHANDLER_H

#include "sequoia-engine/Core/Export.h"
#include <string>

namespace sequoia {

namespace core {

/// @brief Global error handler
/// @ingroup core
class SEQUOIA_API ErrorHandler {
public:
  ErrorHandler() = delete;

  /// @brief Function pointer type of the fatal error handler
  ///
  /// @param message    Message of the error
  /// @param crash      Indicate whether this is a crash (e.g from an assert) or an error
  /// @{
  using FatalErrorHandler = void (*)(std::string, bool);
  using FatalErrorHandlerW = void (*)(std::wstring, bool);
  /// @}

  /// @brief Set the fatal error handler which is invoked when calling `fatal`
  ///
  /// The default error handler will report to `stderr` and, depending if this is a crash, will call
  /// `std::exit(1)` or `std::abort` respectively.
  /// @{
  static void setFatalErrorHandler(FatalErrorHandler handler);
  static void setFatalErrorHandler(FatalErrorHandlerW handler);
  /// @}

  /// @brief Issue a fatal, usually unrecoverable, error
  ///
  /// Use `ErrorHandler::setFatalErrorHandler()` to change the the error handler.
  ///
  /// @param message  Message to display
  /// @param crash    Indicate whether this is a crash (e.g from an assert) or an error
  static void fatal(std::string message, bool crash = false);
  static void fatal(std::wstring message, bool crash = false);
};

} // namespace core

using ErrorHandler = core::ErrorHandler;

} // namespace sequoia

#endif
