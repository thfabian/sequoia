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

#ifndef SEQUOIA_CORE_LOGGING_H
#define SEQUOIA_CORE_LOGGING_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Listenable.h"
#include "sequoia/Core/Singleton.h"
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

namespace sequoia {

namespace core {

/// @enum LoggingLevel
/// @brief Severity levels
/// @ingroup core
enum class LoggingLevel { Debug = 0, Info, Warning, Error, Disabled };

/// @brief Logger listener
/// @ingroup core
class SEQUOIA_API LoggerListener {
public:
  virtual ~LoggerListener() {}

  /// @brief Get current date-time (up to ms accuracy) in the format "hour:min:sec:ms"
  static std::string getCurrentTime();

  /// @brief Get current thread-id as a string
  static std::string getThreadID();

  /// @brief Log `message` of severity `level` at position file:line
  ///
  /// @param level     Severity level
  /// @param message   Message to log
  /// @param file      File from which the logging was issued
  /// @param line      Line in `file` from which the logging was issued
  virtual void loggerLog(LoggingLevel level, const std::string& message, const char* file,
                         int line) = 0;
};

namespace internal {

class SEQUOIA_API LoggerProxy {
  std::mutex* mutex_;        ///< Access mutex
  const LoggingLevel level_; ///< Logging level
  std::stringstream* ss_;    ///< String stream to buffer the logging
  const char* file_;         ///< File from which the logging was issued
  const int line_;           ///< Line in `file` from which the logging was issued
  const bool isNullLogger_;  ///< Is this a pass trough logging?

public:
  LoggerProxy(const LoggerProxy&) = default;
  LoggerProxy(LoggerProxy&&) = default;

  LoggerProxy(std::mutex* lock, LoggingLevel level, std::stringstream* ss, const char* file,
              int line);
  LoggerProxy();
  ~LoggerProxy();

  template <class StreamableValueType>
  inline LoggerProxy& operator<<(StreamableValueType&& value) {
    if(!isNullLogger_)
      (*ss_) << value;
    return *this;
  }
};

} // namespace internal

/// @brief Logger
///
/// To make use of the Logger class, you are required to register yourself as a LoggerListner via
/// `addLoggerListener`. By default no listener is registered and no logging is performed (i.e there
/// is no overhead of logging if no listener is registered).
///
/// The following snippet can be seen as a minimal working example:
///
/// @code{.cpp}
///   #include <sequoia/Core/Logging.h>
///   #include <iostream>
///
///   using sequoia::core;
///
///   class MyLogListener : LoggerListener {
///   public:
///      void log(LoggingLevel level, const std::string& message, const char* file, int line) {
///        std::cout << file << ":" << line << " " << message << std::endl;
///   };
///
///   int main() {
///     auto logger = std::make_unique<Logger>();
///
///     MyLogListener* myLogger = new MyLogger;
///     Logger::getSingleton().addListener(myLogger);
///
///     LOG(INFO) << "Hello world!";
///     delete myLogger;
///   }
/// @endcode
///
/// @threadsafe
/// @ingroup core
class SEQUOIA_API Logger : public Singleton<Logger>, public Listenable<LoggerListener> {
  const LoggingLevel level_; ///< Level at which logging is performed
  std::stringstream ss_;     ///< String stream used to buffer the logging
  std::mutex mutex_;         ///< Mutex for serialized access

public:
  /// @brief Initialize the Logger with a level at which logging will be performed
  /// @param levelStr     Should be one of `{"Debug", "Info", "Warning", "Error", "Disabled"}`
  Logger(const std::string& levelStr);
  Logger(LoggingLevel level);

  /// @name Logging
  /// @{
  internal::LoggerProxy logNull() noexcept { return internal::LoggerProxy(); }
  internal::LoggerProxy logDebug(const char* file, int line) noexcept;
  internal::LoggerProxy logInfo(const char* file, int line) noexcept;
  internal::LoggerProxy logWarning(const char* file, int line) noexcept;
  internal::LoggerProxy logError(const char* file, int line) noexcept;
  /// @}

  /// @brief Check if logging if we need to perform actual logging or if we can return a NullLogger
  template <LoggingLevel Level>
  inline bool requiresLogging() const noexcept {
    return (Level >= level_ && getNumListener<LoggerListener>() > 0);
  }

  /// @brief Get the current LoggingLevel
  inline LoggingLevel getLevel() const noexcept { return level_; }

  /// @brief Calls `LoggerListener::log` of all listeners
  void log(LoggingLevel level, const std::string& message, const char* file, int line);

protected:
  internal::LoggerProxy logImpl(const char* file, int line, LoggingLevel level) noexcept;
};

} // namespace core

/// @brief Loggging macro
/// @threadsafe Logging is thread-safe
/// @ingroup core
#ifdef LOG
#error "LOG already defined"
#endif
#define LOG(Level) SEQUOIA_LOG_##Level##_IMPL()

#define SEQUOIA_LOG_IMPL(Level)                                                                    \
  (sequoia::core::Logger::getSingleton().requiresLogging<sequoia::core::LoggingLevel::Level>()     \
       ? sequoia::core::Logger::getSingleton().log##Level(__FILE__, __LINE__)                      \
       : sequoia::core::Logger::getSingleton().logNull())
#define SEQUOIA_LOG_DEBUG_IMPL() SEQUOIA_LOG_IMPL(Debug)
#define SEQUOIA_LOG_INFO_IMPL() SEQUOIA_LOG_IMPL(Info)
#define SEQUOIA_LOG_WARNING_IMPL() SEQUOIA_LOG_IMPL(Warning)
#define SEQUOIA_LOG_ERROR_IMPL() SEQUOIA_LOG_IMPL(Error)

} // namespace sequoia

#endif
