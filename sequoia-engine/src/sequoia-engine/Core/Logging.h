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

#ifndef SEQUOIA_ENGINE_CORE_LOGGING_H
#define SEQUOIA_ENGINE_CORE_LOGGING_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Listenable.h"
#include "sequoia-engine/Core/Mutex.h"
#include "sequoia-engine/Core/Singleton.h"
#include <sstream>
#include <string>
#include <vector>

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Mutex.h"
#include "sequoia-engine/Core/Singleton.h"
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

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
  SpinMutex* mutex_;           ///< Access mutex
  const LoggingLevel level_;   ///< Logging level
  std::stringstream* ss_;      ///< String stream to buffer the logging
  const char* file_;           ///< File from which the logging was issued
  const int line_;             ///< Line in `file` from which the logging was issued
  const bool isNotNullLogger_; ///< Is this a pass through logging?

public:
  LoggerProxy(const LoggerProxy&) = default;
  LoggerProxy(LoggerProxy&&) = default;

  LoggerProxy(SpinMutex* lock, LoggingLevel level, std::stringstream* ss, const char* file,
              int line);
  LoggerProxy();
  ~LoggerProxy();

  template <class StreamableValueType>
  inline LoggerProxy& operator<<(StreamableValueType&& value) {
    if(isNotNullLogger_)
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
/// @threadsafe Access to this object is thread-safe.
/// @ingroup core
class SEQUOIA_API Logger : public Singleton<Logger>, public Listenable<LoggerListener> {
  const LoggingLevel level_; ///< Level at which logging is performed
  std::stringstream ss_;     ///< String stream used to buffer the logging
  SpinMutex mutex_;          ///< Mutex for serialized access

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

/// @brief Logger of the sequoia-engine
///
/// @ingroup core
class SEQUOIA_API Logger2 : public Singleton<Logger2> {
public:
  friend class Log;
  ~Logger2();

  /// @brief Log to file
  using FileSink = spdlog::sinks::simple_file_sink_mt;

/// @brief Log to stdout
#ifdef SEQUOIA_ON_WIN32
  using StdoutSink = spdlog::sinks::wincolor_stdout_sink_mt;
#else
  using StdoutSink = spdlog::sinks::ansicolor_stderr_sink_mt;
#endif

  /// @brief Create an stdout color sink with customized colors
  static std::shared_ptr<StdoutSink> makeStdoutSink();

  /// @brief Initialize the Logger with a level at which logging will be performed and a default
  /// sink which logs to `stdout`
  ///
  /// @param level    Logging will be performed if the level of the message is `>=` to `level`
  /// @param sink     Default sink (if `nullptr` is passed or `level` is `off` no sink will be
  ///                 registered). Note this should be a multi-threaded i.e **thread-safe** sink.
  Logger2(spdlog::level::level_enum level = spdlog::level::info,
          const spdlog::sink_ptr& sink = makeStdoutSink());

  /// @brief Register the sink `name`
  void addSink(const std::string& name, const spdlog::sink_ptr& sink);

  /// @brief Unregister the sink `name`
  void removeSink(const std::string& name, const spdlog::sink_ptr& sink);

private:
  /// @brief
  inline std::shared_ptr<spdlog::logger>& getLogger() noexcept { return logger_; }

  /// @brief Register the `logger_` with `sinks_`
  void makeLogger();

private:
  SpinMutex mutex_;                                         ///< Mutex for state changes
  spdlog::level::level_enum level_;                         ///< Logging threshold
  std::unordered_map<std::string, spdlog::sink_ptr> sinks_; ///< Registered sinks
  std::shared_ptr<spdlog::logger> logger_;                  ///< Currently active logger
};

/// @brief Logging interface of the sequoia-engine
///
/// @note The `Logger` needs to be instantiated to use these logging methods.
/// @ingroup core
class Log {
public:
  Log() = delete;

  /// @brief Log a trace message
  template <class... Args>
  inline static void trace(const char* format, Args&&... args) {
    Logger2::getSingleton().getLogger()->trace(format, std::forward<Args>(args)...);
  }

  /// @brief Log a debug message
  template <class... Args>
  inline static void debug(const char* format, Args&&... args) {
    Logger2::getSingleton().getLogger()->debug(format, std::forward<Args>(args)...);
  }

  /// @brief Log an info message
  template <class... Args>
  inline static void info(const char* format, Args&&... args) {
    Logger2::getSingleton().getLogger()->info(format, std::forward<Args>(args)...);
  }

  /// @brief Log a warning message
  template <class... Args>
  inline static void warn(const char* format, Args&&... args) {
    Logger2::getSingleton().getLogger()->warn(format, std::forward<Args>(args)...);
  }

  /// @brief Log an error message
  template <class... Args>
  inline static void error(const char* format, Args&&... args) {
    Logger2::getSingleton().getLogger()->error(format, std::forward<Args>(args)...);
  }
};

} // namespace core

using Log = core::Log;

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
