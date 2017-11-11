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

#include "sequoia-engine/Core/CommandLine.h"
#include "sequoia-engine/Core/ErrorHandler.h"
#include "sequoia-engine/Core/Version.h"
#include "sequoia-engine/Unittest/BenchmarkEnvironment.h"
#include <benchmark/benchmark.h>

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(unittest::BenchmarkEnvironment);

namespace unittest {

BenchmarkEnvironment::BenchmarkEnvironment(int argc, char* argv[]) {
  // Push the main options
  options_.push(Options{});
  Options& opt = options_.top();

  // Parse command-line
  core::CommandLine cl("Sequoia Benchmark", core::getSequoiaEngineFullVersionString());
  cl.parse(&opt, argc, argv);

  // Initialize benchmark
  benchmark::Initialize(&argc, argv);

  // By default only log warnings and errors
  logger_ = std::make_unique<core::Logger>(opt.getBool("Core.Debug") ? core::Logger::Trace
                                                                     : core::Logger::Warn,
                                           core::Logger::makeStdoutSink());
}

BenchmarkEnvironment::~BenchmarkEnvironment() {}

void BenchmarkEnvironment::SetUp() {}

void BenchmarkEnvironment::TearDown() {}

} // namespace unittest

} // namespace sequoia
