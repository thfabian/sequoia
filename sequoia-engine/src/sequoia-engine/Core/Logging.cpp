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

#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Logging.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(core::Logger);

namespace core {

Logger::Logger(Level level, const spdlog::sink_ptr& sink) : logger_(nullptr) {
  setLevel(level);
  if(sink)
    sinks_.emplace("default", sink);
  makeLogger();
}

void Logger::addSink(const std::string& name, const std::shared_ptr<LogSink>& sink) {
  SEQUOIA_LOCK_GUARD(mutex_);
  sinks_[name] = sink;
  makeLogger();
}

void Logger::removeSink(const std::string& name) {
  SEQUOIA_LOCK_GUARD(mutex_);
  sinks_.erase(name);
  makeLogger();
}

void Logger::setLevel(Logger::Level level) noexcept {
  SEQUOIA_LOCK_GUARD(mutex_);
  level_ = static_cast<spdlog::level::level_enum>(level);
  makeLogger();
}

Logger::Level Logger::getLevel() const noexcept {
  SEQUOIA_LOCK_GUARD(mutex_);
  return static_cast<Level>(level_);
}

void Logger::makeLogger() {
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

Logger::~Logger() {
  sinks_.clear();
  logger_.reset();
  
  // This causes write to uninitialized memory according to valgrind
  //spdlog::drop_all();
}

std::shared_ptr<Logger::LogSink> Logger::makeStdoutSink() {
  auto sink = std::make_shared<StdoutSink>();
#ifdef SEQUOIA_ON_WIN32
  sink->setColor(spdlog::level::trace, sink->WHITE | sink->BOLD);
  sink->setColor(spdlog::level::debug, sink->WHITE | sink->BOLD);
  sink->setColor(spdlog::level::warn, sink->CYAN | sink->BOLD);
#else
  sink->set_color(spdlog::level::trace, sink->reset);
  sink->set_color(spdlog::level::debug, sink->reset);
  sink->set_color(spdlog::level::warn, sink->magenta + sink->bold);
#endif
  return sink;
}

} // namespace core

} // namespace sequoia
