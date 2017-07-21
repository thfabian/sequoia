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

#include "sequoia/Unittest/Environment.h"
#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Driver/CommandLine.h"
#include "sequoia/Driver/ConsoleLogger.h"
#include "sequoia/Unittest/Config.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(unittest::Environment);

namespace unittest {

Environment::Environment(int argc, char* argv[]) : trace_() {
  singletonManager_ = std::make_unique<core::SingletonManager>();
  singletonManager_->allocateSingleton<ErrorHandler>(argc > 0 ? argv[0] : "SequoiaTest");

  // Initialize options
  singletonManager_->allocateSingleton<Options>();

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
    std::cout << "\nSequoia Unittests (" << SEQUOIA_VERSION_STRING << ")\n\n" << desc << std::endl;
    std::exit(EXIT_SUCCESS);
  }

  // Unittesting always runs in debug mode and with logging on
  singletonManager_->allocateSingleton<core::Logger>(
      vm.count("no-log") ? core::LoggingLevel::Disabled : core::LoggingLevel::Debug);

  if(!vm.count("no-log"))
    singletonManager_->allocateSingleton<driver::ConsoleLogger>();

  Options::getSingleton().Core.Debug = !vm.count("no-debug");

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

std::shared_ptr<File> Environment::getFile(const char* path) const {
  return std::make_shared<TestFile>(path);
}

} // namespace unittest

} // namespace sequoia
