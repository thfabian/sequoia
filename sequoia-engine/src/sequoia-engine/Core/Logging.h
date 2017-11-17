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
#include "sequoia-engine/Core/Mutex.h"
#include "sequoia-engine/Core/Singleton.h"
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

namespace sequoia {

namespace core {

namespace internal {

#ifdef SEQUOIA_ON_WIN32

template <class MutexT>
class WinStdoutSink : public spdlog::sinks::wincolor_stdout_sink<MutexT> {
public:
  using base = spdlog::sinks::wincolor_stdout_sink<MutexT>;

  /// @brief Wrapper to set the level as it is declared protected in the base class
  void setColor(spdlog::level::level_enum level, WORD color) { base::set_color(level, color); }
};

#endif

} // namespace internal

/// @brief Logger of the sequoia-engine
///
/// Note that all these methods are thread-safe.
///
/// @see Log
/// @ingroup core
class SEQUOIA_API Logger : public Singleton<Logger> {
public:
  friend class Log;
  ~Logger();

  /// @brief Logging levels
  enum Level {
    Trace = spdlog::level::trace,
    Debug = spdlog::level::debug,
    Info = spdlog::level::info,
    Warn = spdlog::level::warn,
    Error = spdlog::level::err,
    Disabled = spdlog::level::off
  };

  /// @brief Base-class of all sinks
  using LogSink = spdlog::sinks::sink;

  /// @brief Log to file
  using FileSink = spdlog::sinks::simple_file_sink<SpinMutex>;

/// @brief Log to stdout
#ifdef SEQUOIA_ON_WIN32
  using StdoutSink = internal::WinStdoutSink<SpinMutex>;
#else
  using StdoutSink = spdlog::sinks::ansicolor_stderr_sink<SpinMutex>;
#endif

  /// @brief Create an stdout color sink with customized colors
  static std::shared_ptr<LogSink> makeStdoutSink();

  /// @brief Initialize the Logger with a level at which logging will be performed and a default
  /// sink which logs to `stdout`
  ///
  /// @param level    Logging will be performed if the level of the message is `>=` to `level`
  /// @param sink     Default sink (if `nullptr` is passed or `level` is `off` no sink will be
  ///                 registered). Note this should be a multi-threaded i.e **thread-safe** sink.
  Logger(Level level = Info, const std::shared_ptr<LogSink>& sink = makeStdoutSink());

  /// @brief Register the sink `name`
  void addSink(const std::string& name, const std::shared_ptr<LogSink>& sink);

  /// @brief Unregister the sink `name`
  void removeSink(const std::string& name);

  /// @brief Readjust the logging level
  void setLevel(Level level) noexcept;

  /// @brief Get the current logging level
  Level getLevel() const noexcept;

private:
  /// @brief Get the underlying spd-logger
  inline std::shared_ptr<spdlog::logger>& getLogger() noexcept { return logger_; }

  /// @brief Register the `logger_` with `sinks_`
  void makeLogger();

private:
  mutable SpinMutex mutex_;                                 ///< Mutex for state changes
  spdlog::level::level_enum level_;                         ///< Logging threshold
  std::shared_ptr<spdlog::logger> logger_;                  ///< Currently active logger
  std::unordered_map<std::string, spdlog::sink_ptr> sinks_; ///< Registered sinks
};

/// @brief Logging interface of the sequoia-engine
///
/// To use the `Log` interface, a `Logger` needs to be instantiated and kept alive for the duration
/// of the logging. The `Log` interface accesses this logger via a Singleton pattern.
///
/// A minimal working example is shown below.
///
/// @code{.cpp}
///   // Create a logger which logs everything above and including warnings to stdout
///   auto logger = std::make_unique<Logger>(Logger::Warn, Logger::makeStdoutSink());
///
///   // Log a warning message
///   Log::warn("hello world!");
///
///   // Log an info message (not displayes as the info level is below the warning)
///   Log::info("the answer is {}", 42);
/// @endcode
///
/// @see Logger
/// @ingroup core
class Log {
public:
  Log() = delete;

  /// @brief Log a trace message
  template <class... Args>
  inline static void trace(const char* format, Args&&... args) {
    Logger::getSingleton().getLogger()->trace(format, std::forward<Args>(args)...);
  }

  /// @brief Log a debug message
  template <class... Args>
  inline static void debug(const char* format, Args&&... args) {
    Logger::getSingleton().getLogger()->debug(format, std::forward<Args>(args)...);
  }

  /// @brief Log an info message
  template <class... Args>
  inline static void info(const char* format, Args&&... args) {
    Logger::getSingleton().getLogger()->info(format, std::forward<Args>(args)...);
  }

  /// @brief Log a warning message
  template <class... Args>
  inline static void warn(const char* format, Args&&... args) {
    Logger::getSingleton().getLogger()->warn(format, std::forward<Args>(args)...);
  }

  /// @brief Log an error message
  template <class... Args>
  inline static void error(const char* format, Args&&... args) {
    Logger::getSingleton().getLogger()->error(format, std::forward<Args>(args)...);
  }
};

} // namespace core

using Log = core::Log;

} // namespace sequoia

#endif
