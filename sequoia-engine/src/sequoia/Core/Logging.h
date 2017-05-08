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

#ifndef SEQUOIA_CORE_LOGGING
#define SEQUOIA_CORE_LOGGING

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
enum class LoggingLevel { Info = 0, Warning, Error, Fatal };

/// @brief Logger listener
/// @ingroup core
class SEQUOIA_CORE_API LoggerListener {
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

class SEQUOIA_CORE_API LoggerProxy {
  std::mutex* lock_;
  const LoggingLevel level_;
  std::stringstream* ss_;
  const char* file_;
  const int line_;
  const bool isNullLogger_;

public:
  LoggerProxy(const LoggerProxy&) = default;
  LoggerProxy(std::mutex* lock, LoggingLevel level, std::stringstream* ss, const char* file,
              int line);
  LoggerProxy();
  ~LoggerProxy();

  template <class StreamableValueType>
  LoggerProxy& operator<<(StreamableValueType&& value) {
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
class SEQUOIA_CORE_API Logger : public Singleton<Logger>, public Listenable<LoggerListener> {
  std::stringstream ss_;
  std::mutex lock_;

public:
  /// @name Logging
  /// @{
  internal::LoggerProxy logNull() noexcept { return internal::LoggerProxy(); }
  internal::LoggerProxy logInfo(const char* file, int line) noexcept;
  internal::LoggerProxy logWarning(const char* file, int line) noexcept;
  internal::LoggerProxy logError(const char* file, int line) noexcept;
  internal::LoggerProxy logFatal(const char* file, int line) noexcept;
  /// @}

  /// @brief Check if there are any listeners registered
  bool hasListeners() const noexcept { return getNumListener<LoggerListener>() > 0; }

  /// @brief Calls `LoggerListener::log` of all listeners
  void log(LoggingLevel level, const std::string& message, const char* file, int line);
};

} // namespace core

/// @macro LOG
/// @brief Loggging macro
///
/// @threadsafe Logging is thread-safe
/// @ingroup core
#ifdef LOG
#error "LOG already defined"
#endif
#define LOG(Level) SEQUOIA_LOG_##Level##_IMPL()

#define SEQUOIA_LOG_IMPL(func)                                                                     \
  (sequoia::core::Logger::getSingleton().hasListeners()                                            \
       ? sequoia::core::Logger::getSingleton().func(__FILE__, __LINE__)                            \
       : sequoia::core::Logger::getSingleton().logNull())
#define SEQUOIA_LOG_INFO_IMPL() SEQUOIA_LOG_IMPL(logInfo)
#define SEQUOIA_LOG_WARNING_IMPL() SEQUOIA_LOG_IMPL(logWarning)
#define SEQUOIA_LOG_ERROR_IMPL() SEQUOIA_LOG_IMPL(logError)
#define SEQUOIA_LOG_FATAL_IMPL() SEQUOIA_LOG_IMPL(logFatal)

} // namespace sequoia

#endif
