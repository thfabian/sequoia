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

#include "sequoia-engine/Unittest/TestEnvironment.h"
#include "sequoia-engine/Core/ErrorHandler.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringSwitch.h"
#include "sequoia-engine/Core/Version.h"
#include "sequoia-engine/Driver/ConsoleLogger.h"
#include "sequoia-engine/Unittest/Config.h"
#include "sequoia-engine/Unittest/TestFile.h"
#include "sequoia-engine/Unittest/TestOptions.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(unittest::TestEnvironment);

namespace unittest {

TestEnvironment::TestEnvironment(int argc, char* argv[], render::RenderSystemKind kind)
    : trace_(), renderSystemKind_(kind) {
  singletonManager_ = std::make_unique<core::SingletonManager>();
  //singletonManager_->allocateSingleton<ErrorHandler>(argc > 0 ? argv[0] : "SequoiaTest");

  // Initialize test options
  singletonManager_->allocateSingleton<TestOptions>();

  // Parse command-line
  po::options_description desc("Unittest options");
  desc.add_options()
      // --help
      ("help", "Display this information.")
      // --no-debug
      ("no-debug", "Disable debug mode.")
      // --no-log
      ("no-log", "Disable logging.")
      // -r, --renderer
      ("renderer,r", po::value<std::string>()->value_name("RENDERER"),
       "Renderer to use, where <RENDERER> is one of [gl, null].");

  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch(std::exception& e) {
    ErrorHandler::fatal(e.what(), false);
  }

  if(vm.count("help")) {
    std::cout << "\nSequoia Unittests (" << core::getSequoiaEngineFullVersionString() << ")\n\n"
              << desc << std::endl;
    std::exit(EXIT_SUCCESS);
  }

  // Set the preferred RenderSystem
  if(renderSystemKind_ == render::RK_Invalid) {
    if(vm.count("renderer"))
      renderSystemKind_ =
          core::StringSwitch<render::RenderSystemKind>(vm["renderer"].as<std::string>())
              .Case("gl", render::RK_OpenGL)
              .Case("null", render::RK_Null)
              .Default(render::RK_Invalid);
    else
      renderSystemKind_ = render::RK_Null;
  }
  
  if(renderSystemKind_ == render::RK_Invalid)
    ErrorHandler::fatal(core::format("invalid value '{}' of option '--render'",
                                     vm["renderer"].as<std::string>()),
                        false);

  // Unittesting always runs in debug mode and with logging on
  singletonManager_->allocateSingleton<core::Logger>(
      vm.count("no-log") ? core::LoggingLevel::Disabled : core::LoggingLevel::Debug);

  if(!vm.count("no-log"))
    singletonManager_->allocateSingleton<driver::ConsoleLogger>();

  TestOptions::getSingleton().Core.Debug = !vm.count("no-debug");

  ressourcePath_ = SEQUOIA_ENGINE_UNITTEST_RESSOURCEPATH;
  temporaryPath_ = SEQUOIA_ENGINE_UNITTEST_TEMPORARYPATH;

  if(!platform::filesystem::exists(ressourcePath_))
    ErrorHandler::fatal(PLATFORM_STR("invalid ressource path: '") +
                                     ressourcePath_.native() + PLATFORM_STR("'"));

  // Take snapshot of the options
  TestOptions::getSingleton().save();
}

TestEnvironment::~TestEnvironment() {}

void TestEnvironment::SetUp() { TestOptions::getSingleton().restoreFirstSnapshot(); }

void TestEnvironment::TearDown() {}

std::string TestEnvironment::testCaseName() const {
  const ::testing::TestInfo* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
  if(testInfo)
    return testInfo->test_case_name();
  return "";
}

std::string TestEnvironment::testName() const {
  const ::testing::TestInfo* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
  if(testInfo)
    return testInfo->name();
  return "";
}

std::shared_ptr<File> TestEnvironment::getFile(const char* path) const {
  return std::make_shared<TestFile>(getRessourcePath() / platform::asPath(path));
}

std::shared_ptr<File> TestEnvironment::createFile(const char* path) const {
  auto filepath = getTemporaryPath() / platform::asPath(path);
  platform::filesystem::create_directories(filepath.parent_path());
  return std::make_shared<TestFile>(filepath);
}

} // namespace unittest
} // namespace sequoia
