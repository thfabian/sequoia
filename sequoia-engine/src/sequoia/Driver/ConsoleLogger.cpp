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

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Core/UtfString.h"
#include "sequoia/Driver/ConsoleLogger.h"
#include <fstream>
#include <iostream>

#define SEQUOIA_LOGGING_PRINT_FILE 0

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(driver::ConsoleLogger);

namespace driver {

ConsoleLogger::ConsoleLogger(std::string file) : stream_(nullptr), file_(nullptr) {
  if(!file.empty()) {
    // Try the open the file
    file_ = std::make_unique<std::ofstream>(file);

    if(!file_->is_open()) {
      ErrorHandler::getSingleton().warning("failed to open log file: \"" + file + "\"");
      file_.release();

      // ... failed log to temporary file instead
      platform::Path path =
          platform::filesystem::temp_directory_path() / platform::filesystem::unique_path();
      auto tempFile = UtfString(path.native()).toAnsiString();
      ErrorHandler::getSingleton().warning(PLATFORM_STR("logging to file: \"") + path.native() +
                                           PLATFORM_STR("\" instead"));

      // Opening temporary file failed as well... we give up!
      file_ = std::make_unique<std::ofstream>(tempFile.c_str());
      if(!file_->is_open()) {
        ErrorHandler::getSingleton().warning("failed to open temp log file: \"" + tempFile + "\"");
        return;
      }
    }
    stream_ = file_.get();
  } else
    stream_ = &std::cout;

  // Register as LoggerListener
  core::Logger::getSingleton().addListener(static_cast<LoggerListener*>(this));
}

ConsoleLogger::~ConsoleLogger() {
  core::Logger::getSingleton().removeListener(static_cast<LoggerListener*>(this));
}

void ConsoleLogger::log(core::LoggingLevel level, const std::string& message, const char* file,
                        int line) {
  if(stream_) {
    (*stream_) << "[" << LoggerListener::getCurrentTime() << "] ";

    switch(level) {
    case core::LoggingLevel::Info:
      (*stream_) << "[INFO]";
      break;
    case core::LoggingLevel::Warning:
      (*stream_) << "[WARN]";
      break;
    case core::LoggingLevel::Error:
      (*stream_) << "[ERROR]";
      break;
    case core::LoggingLevel::Fatal:
      (*stream_) << "[FATAL]";
      break;
    }

    (*stream_) << " [" << LoggerListener::getThreadID() << "] ";

#if SEQUOIA_LOGGING_PRINT_FILE
    platform::Path path(file);
#ifdef SEQUOIA_ON_WIN32
    (*stream_) << "[" << UtfString(path.filename().native()).toAnsiString() << ":" << line << "] ";
#else
    (*stream_) << "[" << path.filename().native() << ":" << line << "]";
#endif
#endif

    (*stream_) << message << "\n";
  }
}

} // namespace driver

} // namespace sequoia
