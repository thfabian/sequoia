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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
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
  std::ostringstream ss;
  ss << std::hex << std::this_thread::get_id();
  return ss.str();
}

internal::LoggerProxy::LoggerProxy(std::mutex* lock, LoggingLevel level, std::stringstream* ss,
                                   const char* file, int line)
    : lock_(lock), level_(level), ss_(ss), file_(file), line_(line), isNullLogger_(false) {}

internal::LoggerProxy::LoggerProxy()
    : lock_(nullptr), level_(LoggingLevel::Info), ss_(nullptr), file_(nullptr), line_(0),
      isNullLogger_(true) {}

internal::LoggerProxy::~LoggerProxy() {
  if(!isNullLogger_) {
    Logger::getSingleton().log(level_, ss_->str(), file_, line_);
    ss_->str("");
    ss_->clear();
    lock_->unlock();
  }
}

internal::LoggerProxy Logger::logInfo(const char* file, int line) noexcept {
  lock_.lock();
  return internal::LoggerProxy(&lock_, LoggingLevel::Info, &ss_, file, line);
}

internal::LoggerProxy Logger::logWarning(const char* file, int line) noexcept {
  lock_.lock();
  return internal::LoggerProxy(&lock_, LoggingLevel::Warning, &ss_, file, line);
}

internal::LoggerProxy Logger::logError(const char* file, int line) noexcept {
  lock_.lock();
  return internal::LoggerProxy(&lock_, LoggingLevel::Error, &ss_, file, line);
}

internal::LoggerProxy Logger::logFatal(const char* file, int line) noexcept {
  lock_.lock();
  return internal::LoggerProxy(&lock_, LoggingLevel::Fatal, &ss_, file, line);
}

void Logger::log(LoggingLevel level, const std::string& message, const char* file, int line) {
  for(auto* listeners : getListeners<LoggerListener>())
    listeners->log(level, message, file, line);
}

} // namespace core

} // namespace sequoia
