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
#include "sequoia/Unittest/Environment.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(unittest::Environment);

namespace unittest {

Environment::Environment(int argc, char* argv[]) {

  singletonManager_ = std::make_unique<core::SingletonManager>();
  singletonManager_->allocateSingleton<ErrorHandler>(argc > 0 ? argv[0] : "SequoiaTest");
  singletonManager_->allocateSingleton<core::Logger>();

  // Parse command-line
  po::options_description desc("Unittest options");
  desc.add_options()("help", "Display this information.")("logging", "Enable logging");

  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch(std::exception& e) {
    ErrorHandler::getSingleton().fatal(e.what(), false);
  }

  if(vm.count("help")) {
    std::cout << "\nSequoiaUnittest - " << SEQUOIA_VERSION_STRING << "\n\n" << desc << std::endl;
    std::exit(0);
  }

  if(vm.count("logging"))
    singletonManager_->allocateSingleton<driver::ConsoleLogger>();
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

} // namespace unittest

} // namespace sequoia
