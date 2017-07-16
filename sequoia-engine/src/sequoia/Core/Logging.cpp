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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/StringSwitch.h"
#include <chrono>
#include <ctime>
#include <thread>

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(core::Logger);

namespace core {

std::string LoggerListener::getCurrentTime() {
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  auto now_ms = now.time_since_epoch();
  auto now_sec = std::chrono::duration_cast<std::chrono::seconds>(now_ms);
  auto tm_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now_ms - now_sec);

  std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
  struct tm* localTime = std::localtime(&currentTime);

  return format("%02i:%02i:%02i.%03i", localTime->tm_hour, localTime->tm_min, localTime->tm_sec,
                tm_ms.count());
}

std::string LoggerListener::getThreadID() {
  return core::format("0x%x", std::this_thread::get_id());
}

internal::LoggerProxy::LoggerProxy(SpinMutex* mutex, LoggingLevel level, std::stringstream* ss,
                                   const char* file, int line)
    : mutex_(mutex), level_(level), ss_(ss), file_(file), line_(line), isNullLogger_(false) {}

internal::LoggerProxy::LoggerProxy()
    : mutex_(nullptr), level_(LoggingLevel::Info), ss_(nullptr), file_(nullptr), line_(0),
      isNullLogger_(true) {}

internal::LoggerProxy::~LoggerProxy() {
  if(!isNullLogger_) {
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

} // namespace core

} // namespace sequoia
