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

#ifndef SEQUOIA_EDITOR_CORE_QTERRORHANDLER_H
#define SEQUOIA_EDITOR_CORE_QTERRORHANDLER_H

#include "sequoia-editor/Core/Export.h"
#include "sequoia-engine/Core/Singleton.h"
#include <string>

namespace sequoia_editor {

namespace core {

/// @brief Global error handler
/// @ingroup core
class SEQUOIA_EDITOR_API QtErrorHandler : public sequoia::Singleton<QtErrorHandler> {
public:
  /// @brief Install the Qt error handler
  QtErrorHandler();

  /// @brief Restore the default error handlers
  ~QtErrorHandler();

  /// @brief Issue a fatal, unrecoverable error in a separate window
  ///
  /// @param message  Message to display
  /// @param crash    Indicate whether this is a crash (e.g from an assert) or an error (a crash
  ///                 will call `std::abort` while an error will call `std::exit(1)`).
  void fatal(std::string message, bool crash = false);
  void fatal(std::wstring message, bool crash = false);
};

} // namespace core

using QtErrorHandler = core::QtErrorHandler;

} // namespace sequoia_editor

#endif
