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

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Driver/ConsoleLogger.h"
#include "sequoia/Unittest/Config.h"
#include "sequoia/Unittest/Environment.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(unittest::Environment);

namespace unittest {

Environment::Environment(int argc, char* argv[]) : trace_() {
  singletonManager_ = std::make_unique<core::SingletonManager>();
  singletonManager_->allocateSingleton<ErrorHandler>(argc > 0 ? argv[0] : "SequoiaTest");

  // Parse command-line
  po::options_description desc("Unittest options");
  desc.add_options()
      // --help
      ("help", "Display this information.")
      // --no-debug
      ("no-debug", "Disable debug mode.")
      // --no-log
      ("no-log", "Disable logging.");

  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch(std::exception& e) {
    ErrorHandler::getSingleton().fatal(e.what(), false, false);
  }

  if(vm.count("help")) {
    std::cout << "\nSequoiaUnittest - " << SEQUOIA_VERSION_STRING << "\n\n" << desc << std::endl;
    std::exit(0);
  }

  singletonManager_->allocateSingleton<core::Logger>(
      vm.count("no-log") ? core::LoggingLevel::Disabled : core::LoggingLevel::Debug);

  if(!vm.count("no-log"))
    singletonManager_->allocateSingleton<driver::ConsoleLogger>();

  debugMode_ = !vm.count("no-debug");

  path_ = SEQUOIA_UNITTEST_RESSOURCEPATH;
  if(!platform::filesystem::exists(path_))
    ErrorHandler::getSingleton().fatal(PLATFORM_STR("invalid ressource path: '") + path_.native() +
                                       PLATFORM_STR("'"));
}

Environment::~Environment() {}

void Environment::SetUp() {}

void Environment::TearDown() {}

std::string Environment::testCaseName() const {
  const ::testing::TestInfo* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
  if(testInfo)
    return testInfo->test_case_name();
  return "";
}

std::string Environment::testName() const {
  const ::testing::TestInfo* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
  if(testInfo)
    return testInfo->name();
  return "";
}

const platform::Path& Environment::getRessourcePath() const { return path_; }

bool Environment::debugMode() const { return debugMode_; }

} // namespace unittest

} // namespace sequoia
