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

#ifndef SEQUOIA_ENGINE_DRIVER_CONSOLELOGGER_H
#define SEQUOIA_ENGINE_DRIVER_CONSOLELOGGER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Singleton.h"
#include <iosfwd>
#include <memory>
#include <string>

namespace sequoia {

namespace driver {

/// @brief Console/File Logger
/// @ingroup driver
class SEQUOIA_API ConsoleLogger : public core::LoggerListener, public Singleton<ConsoleLogger> {
  /// std::cout or alias of `file_`
  std::ostream* stream_;

  /// File used for logging (might be `NULL`)
  std::unique_ptr<std::ofstream> file_;

public:
  /// @brief Initialize the logger and register as a listener within the core::Logger
  /// @param file     If non-empty the logger will write to the `file` instead of `stdout`
  ConsoleLogger(std::string file = "");

  ~ConsoleLogger();

  /// @copydoc core::LoggerListener::log
  virtual void loggerLog(core::LoggingLevel level, const std::string& message, const char* file,
                         int line) override;
};

} // namespace driver

} // namespace sequoia

#endif
