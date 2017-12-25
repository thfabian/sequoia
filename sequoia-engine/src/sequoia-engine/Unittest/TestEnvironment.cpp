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
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Core/Version.h"
#include "sequoia-engine/Unittest/Config.h"
#include "sequoia-engine/Unittest/Exception.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include "sequoia-engine/Unittest/TestFile.h"
#include <fstream>
#include <iostream>

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(unittest::TestEnvironment);

namespace unittest {

static platform::Path createTemporaryPathImpl(const platform::Path& temporaryPath,
                                              const platform::Path& path, bool isDir,
                                              std::string content) {
  auto newPath = temporaryPath / path;
  platform::filesystem::create_directories(isDir ? newPath : newPath.parent_path());
  if(!isDir) {
    std::ofstream ofs(platform::toAnsiString(newPath).c_str());
    ofs << content;
  }

  if(!platform::filesystem::exists(newPath))
    SEQUOIA_THROW(UnittestException, "failed to create {}: \"{}\"", isDir ? "directory" : "file",
                  platform::toAnsiString(newPath));

  return newPath;
}

TestEnvironment::TestEnvironment(int argc, char* argv[], render::RenderSystemKind kind)
    : trace_(), renderSystemKind_(kind) {

  options_ = std::make_shared<Options>();

  // Unittests are *always* in debug mode and with logging enabled
  options_->setBool("Unittest.NoDebug", false,
                    core::OptionMetaData{"no-debug", "", false, "", "Disable debug mode"});

  options_->setBool("Unittest.NoLogging", false,
                    core::OptionMetaData{"no-log", "", false, "", "Disable logging"});

  options_->setBool(
      "Unittest.NoTrace", false,
      core::OptionMetaData{"no-trace", "", false, "", "Disable graphics API tracing"});

  options_->setString("Unittest.Renderer", "null",
                      core::OptionMetaData{"render", "r", true, "RENDERER",
                                           "Renderer to use, where RENDERER is one of [gl, null]"});

  // Parse command-line
  core::CommandLine cl("Sequoia Unittest", core::getSequoiaEngineFullVersionString());
  cl.parse(options_.get(), argc, argv);

  // Set debug modes
  options_->setBool("Core.Debug", !options_->getBool("Unittest.NoDebug"));
  options_->setBool("Render.TraceAPI", !options_->getBool("Unittest.NoTrace"));

  // Set logging
  spdlog::sink_ptr sink =
      options_->getBool("Unittest.NoLogging") ? nullptr : core::Logger::makeStdoutSink();
  logger_ = std::make_unique<core::Logger>(core::Logger::Trace, sink);

  // Set the preferred RenderSystem
  if(renderSystemKind_ == render::RK_Invalid) {
    renderSystemKind_ =
        core::StringSwitch<render::RenderSystemKind>(options_->getString("Unittest.Renderer"))
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

void TestEnvironment::SetUp() {}

void TestEnvironment::TearDown() {}

std::string TestEnvironment::testCaseName() const {
  const ::testing::TestInfo* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
  if(testInfo)
    return testInfo->test_case_name();
  sequoia_unreachable("testCaseName() called outside test");
}

std::string TestEnvironment::testName() const {
  const ::testing::TestInfo* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
  if(testInfo)
    return testInfo->name();
  sequoia_unreachable("testName() called outside test");
}

platform::Path
TestEnvironment::createTemporaryDir(const platform::Path& path) const {
  return createTemporaryPathImpl(temporaryPath_, path, true, "");
}

platform::Path TestEnvironment::createTemporaryFile(const platform::Path& path,
                                                    std::string content) const {
  return createTemporaryPathImpl(temporaryPath_, path, false, content);
}

std::shared_ptr<File> TestEnvironment::getFile(const char* path) const {
  return std::make_shared<TestFile>(getRessourcePath() / platform::asPath(path));
}

std::shared_ptr<File> TestEnvironment::createFile(const char* path) const {
  auto filepath = getTemporaryPath() / platform::asPath(path);
  platform::filesystem::create_directories(filepath.parent_path());
  return std::make_shared<TestFile>(filepath);
}

std::shared_ptr<Options> TestEnvironment::getOptions() {
  return std::make_shared<Options>(*options_);
}

} // namespace unittest

} // namespace sequoia
