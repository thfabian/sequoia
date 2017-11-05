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

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringSwitch.h"
#include <chrono>
#include <ctime>
#include <thread>

#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Logging.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(core::Logger);

SEQUOIA_DECLARE_SINGLETON(core::Logger2);

namespace core {

std::string LoggerListener::getCurrentTime() {
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  auto now_ms = now.time_since_epoch();
  auto now_sec = std::chrono::duration_cast<std::chrono::seconds>(now_ms);
  auto tm_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now_ms - now_sec);

  std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
  struct tm* localTime = std::localtime(&currentTime);

  return core::format("{:02d}:{:02d}:{:02d}.{:02d}", localTime->tm_hour, localTime->tm_min,
                      localTime->tm_sec, tm_ms.count());
}

std::string LoggerListener::getThreadID() { return core::format("{}", std::this_thread::get_id()); }

internal::LoggerProxy::LoggerProxy(SpinMutex* mutex, LoggingLevel level, std::stringstream* ss,
                                   const char* file, int line)
    : mutex_(mutex), level_(level), ss_(ss), file_(file), line_(line), isNotNullLogger_(true) {}

internal::LoggerProxy::LoggerProxy()
    : mutex_(nullptr), level_(LoggingLevel::Info), ss_(nullptr), file_(nullptr), line_(0),
      isNotNullLogger_(false) {}

internal::LoggerProxy::~LoggerProxy() {
  if(isNotNullLogger_) {
    Logger::getSingleton().log(level_, ss_->str(), file_, line_);
    ss_->str("");
    ss_->clear();
    mutex_->unlock();
  }
}

Logger::Logger(const std::string& levelStr)
    : level_(StringSwitch<LoggingLevel>(levelStr)
                 .Case("Debug", LoggingLevel::Debug)
                 .Case("Info", LoggingLevel::Info)
                 .Case("Warning", LoggingLevel::Warning)
                 .Case("Error", LoggingLevel::Error)
                 .Case("Disabled", LoggingLevel::Disabled)
                 .Default(LoggingLevel::Info)) {}

Logger::Logger(LoggingLevel level) : level_(level) {}

internal::LoggerProxy Logger::logDebug(const char* file, int line) noexcept {
  return logImpl(file, line, LoggingLevel::Debug);
}

internal::LoggerProxy Logger::logInfo(const char* file, int line) noexcept {
  return logImpl(file, line, LoggingLevel::Info);
}

internal::LoggerProxy Logger::logWarning(const char* file, int line) noexcept {
  return logImpl(file, line, LoggingLevel::Warning);
}

internal::LoggerProxy Logger::logError(const char* file, int line) noexcept {
  return logImpl(file, line, LoggingLevel::Error);
}

internal::LoggerProxy Logger::logImpl(const char* file, int line, LoggingLevel level) noexcept {
  mutex_.lock();
  return internal::LoggerProxy(&mutex_, level, &ss_, file, line);
}

void Logger::log(LoggingLevel level, const std::string& message, const char* file, int line) {
  for(auto* listeners : getListeners<LoggerListener>())
    listeners->loggerLog(level, message, file, line);
}

Logger2::Logger2(spdlog::level::level_enum level, const spdlog::sink_ptr& sink)
    : level_(level), logger_(nullptr) {
  if(sink)
    sinks_.emplace("default", sink);
  makeLogger();
}

void Logger2::addSink(const std::string& name, const spdlog::sink_ptr& sink) {
  SEQUOIA_LOCK_GUARD(mutex_);
  sinks_[name] = sink;
  makeLogger();
}

void Logger2::removeSink(const std::string& name, const spdlog::sink_ptr& sink) {
  SEQUOIA_LOCK_GUARD(mutex_);
  sinks_.erase(name);
  makeLogger();
}

void Logger2::makeLogger() {
  if(logger_)
    spdlog::drop("sequoia");

  std::vector<spdlog::sink_ptr> sinkVec;
  for(const auto& sink : sinks_)
    sinkVec.emplace_back(sink.second);

  try {
    logger_ = std::make_shared<spdlog::logger>("sequoia", sinkVec.begin(), sinkVec.end());
    logger_->set_level(level_);
    logger_->set_pattern("[%H:%M:%S.%e] [%t] [%l] %v");
    spdlog::register_logger(logger_);
  } catch(const spdlog::spdlog_ex& ex) {
    SEQUOIA_THROW(Exception, "cannot create logger: {}", ex.what());
  }
}

Logger2::~Logger2() { spdlog::drop_all(); }

std::shared_ptr<Logger2::StdoutSink> Logger2::makeStdoutSink() {
  auto sink = std::make_shared<StdoutSink>();
  sink->set_color(spdlog::level::trace, sink->reset);
  sink->set_color(spdlog::level::debug, sink->reset);
  sink->set_color(spdlog::level::warn, sink->magenta + sink->bold);
  return sink;
}

} // namespace core

} // namespace sequoia
