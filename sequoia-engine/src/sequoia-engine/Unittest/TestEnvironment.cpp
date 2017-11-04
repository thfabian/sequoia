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
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringSwitch.h"
#include "sequoia-engine/Core/Version.h"
#include "sequoia-engine/Unittest/Config.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include "sequoia-engine/Unittest/TestFile.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(unittest::TestEnvironment);

namespace unittest {

TestEnvironment::TestEnvironment(int argc, char* argv[], render::RenderSystemKind kind)
    : trace_(), renderSystemKind_(kind) {

  // Push the main options
  options_.push(Options{});
  Options& opt = options_.top();

  // Unittests are *always* in debug mode and with logging
  opt.setBool("Unittest.NoDebug", false);
  opt.setMetaData("Unittest.NoDebug",
                  core::OptionMetaData{"no-debug", "", false, "", "Disable debug mode"});
  opt.setBool("Unittest.NoLogging", false);
  opt.setMetaData("Unittest.NoLogging",
                  core::OptionMetaData{"no-log", "", false, "", "Disable logging"});

  // Parse command-line
  opt.setString("Unittest.Renderer", "null");
  opt.setMetaData("Unittest.Renderer",
                  core::OptionMetaData{"render", "r", true, "RENDERER",
                                       "Renderer to use, where RENDERER is one of [gl, null]"});

  core::CommandLine cl("Sequoia Unittest", core::getSequoiaEngineFullVersionString());
  cl.parse(&opt, argc, argv);

  // Update debug mode
  opt.setBool("Core.Debug", !opt.get<bool>("Unittest.NoDebug"));

  // Set logging

  // Set the preferred RenderSystem
  if(renderSystemKind_ == render::RK_Invalid) {
    renderSystemKind_ =
        core::StringSwitch<render::RenderSystemKind>(opt.getAsString("Unittest.Renderer"))
            .Case("gl", render::RK_OpenGL)
            .Case("null", render::RK_Null)
            .Default(render::RK_Invalid);
  }

  ressourcePath_ = SEQUOIA_ENGINE_UNITTEST_RESSOURCEPATH;
  temporaryPath_ = SEQUOIA_ENGINE_UNITTEST_TEMPORARYPATH;

  if(!platform::filesystem::exists(ressourcePath_))
    ErrorHandler::fatal(PLATFORM_STR("invalid ressource path: '") + ressourcePath_.native() +
                        PLATFORM_STR("'"));
}

TestEnvironment::~TestEnvironment() {}

void TestEnvironment::SetUp() { pushOptions(); }

void TestEnvironment::TearDown() { popOptions(); }

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

Options& TestEnvironment::getOption() {
  SEQUOIA_ASSERT_MSG(!options_.empty(), "options are empty");
  return options_.top();
}

void TestEnvironment::pushOptions() { options_.push(options_.top().clone()); }

void TestEnvironment::popOptions() {
  if(options_.size() == 1)
    return;
  options_.pop();
}

} // namespace unittest

} // namespace sequoia
