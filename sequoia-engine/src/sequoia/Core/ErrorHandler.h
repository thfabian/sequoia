//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_ERRORHANDLER_H
#define SEQUOIA_CORE_ERRORHANDLER_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Singleton.h"
#include "sequoia/Core/UtfString.h"

namespace sequoia {

namespace core {

/// @brief Global error handler
/// @ingroup core
class SEQUOIA_CORE_API ErrorHandler : public Singleton<ErrorHandler> {
  UtfString program_;

public:
  /// @brief Setup ErrorHandler
  ///
  /// @param program    Name/Path of the program. If a full path is provided, the filename (
  ///                   without the extension) will be used as program name.
  ErrorHandler(UtfString program);

  /// @brief Report a fatal error and exit with `EXIT_FAILURE`
  ///
  /// @param message      Message to display
  /// @param messagebox   If true display message in a popup GUI (Win32 only), otherwise write to
  ///                     console (`stderr`).
  SEQUOIA_ATTRIBUTE_NORETURN void fatal(std::wstring message, bool messagebox = true) noexcept;
  SEQUOIA_ATTRIBUTE_NORETURN void fatal(std::string message, bool messagebox = true) noexcept;

  /// @brief Report warning to stderr
  ///
  /// @param message      Message to display
  void warning(std::wstring message) noexcept;
  void warning(std::string message) noexcept;

  /// @brief Get program name
  UtfString program() const noexcept;
};

} // namespace core

using ErrorHandler = core::ErrorHandler;

} // namespace sequoia

#endif
