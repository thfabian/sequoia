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

#include "sequoia-engine/Unittest/BenchmarkEnvironment.h"
#include "sequoia-engine/Core/ErrorHandler.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Version.h"
#include "sequoia-engine/Driver/ConsoleLogger.h"
#include "sequoia-engine/Unittest/TestOptions.h"
#include <benchmark/benchmark.h>
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(unittest::BenchmarkEnvironment);

namespace unittest {

BenchmarkEnvironment::BenchmarkEnvironment(int argc, char* argv[]) {
  singletonManager_ = std::make_unique<core::SingletonManager>();
  singletonManager_->allocateSingleton<ErrorHandler>(argc > 0 ? argv[0] : "SequoiaBenchmark");

  // Initialize test options
  singletonManager_->allocateSingleton<TestOptions>();

  // Parse command-line
  po::options_description desc("Benchmark options");
  desc.add_options()
      // --help
      ("help", "Display this information.")
      // --debug
      ("debug", "Enable debug mode.");

  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
    po::notify(vm);
  } catch(std::exception& e) {
    ErrorHandler::getSingleton().fatal(e.what(), false, false);
  }

  if(vm.count("help"))
    std::cout << "\nSequoia Benchmark (" << core::getSequoiaEngineFullVersionString() << ")\n\n"
              << desc << std::endl;

  // Initialize benchmark
  benchmark::Initialize(&argc, argv);

  if(vm.count("help"))
    std::exit(EXIT_SUCCESS);

  // Benchmarking by default disables logging and debugging
  singletonManager_->allocateSingleton<core::Logger>(
      vm.count("debug") ? core::LoggingLevel::Debug : core::LoggingLevel::Disabled);
  if(vm.count("debug"))
    singletonManager_->allocateSingleton<driver::ConsoleLogger>();

  TestOptions::getSingleton().Core.Debug = vm.count("debug");

  // Take snapshot of the options
  TestOptions::getSingleton().save();
}

BenchmarkEnvironment::~BenchmarkEnvironment() {}

void BenchmarkEnvironment::SetUp() { TestOptions::getSingleton().restoreFirstSnapshot(); }

void BenchmarkEnvironment::TearDown() {}

} // namespace unittest

} // namespace sequoia
