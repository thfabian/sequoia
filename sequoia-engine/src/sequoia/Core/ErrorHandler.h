//===-- sequoia/Core/ErrorHandler.h -------------------------------------------------*- C++ -*-===//
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

#ifndef SEQUOIA_CORE_ERRORHANDLER_H
#define SEQUOIA_CORE_ERRORHANDLER_H

#include "sequoia/Core/Core.h"
#include "sequoia/Core/String.h"
#include <OGRE/OgreSingleton.h>

namespace sequoia {

namespace core {

/// @brief Global error handler.
///
/// @ingroup core
class SEQUOIA_EXPORT ErrorHandler : public Ogre::Singleton<ErrorHandler> {
public:
  /// @brief Setup ErrorHandler
  ///
  /// @param program    Name/Path of the program. If a full path is provided, the filename (
  ///                   without the extension) will be used as program name.
  ErrorHandler(String program);

  /// @brief Report a fatal error and exit with `EXIT_FAILURE`
  ///
  /// @param message      Message to display
  /// @param messagebox   If true, display message in a popup GUI (Win32 only) otherwise, write to
  ///                     console (`stderr`).
  SEQUOIA_ATTRIBUTE_NORETURN void fatal(String message, bool messagebox = true) noexcept;
  
  /// @brief Report warning to stderr 
  /// 
  /// @param message      Message to display
  void warning(String message) noexcept;
  
  /// @brief Get program name
  String program() const noexcept; 

private:
  String program_;
};

} // namespace core

using ErrorHandler = core::ErrorHandler;

} // namespace sequoia

#endif
