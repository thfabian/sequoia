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

#ifndef SEQUOIA_UNITTEST_TESTENVIRONMENT_H
#define SEQUOIA_UNITTEST_TESTENVIRONMENT_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Core/PrettyStackTrace.h"
#include "sequoia/Core/Singleton.h"
#include "sequoia/Core/SingletonManager.h"
#include "sequoia/Render/RenderSystemObject.h"
#include "sequoia/Unittest/TestFile.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace sequoia {

namespace unittest {

/// @brief Global test environment
/// @ingroup unittest
class SEQUOIA_API TestEnvironment : public testing::Environment, public Singleton<TestEnvironment> {
public:
  /// @brief Parse command-line
  TestEnvironment(int argc, char* argv[], render::RenderSystemKind kind = render::RK_Invalid);

  /// @brief Virtual destructor
  virtual ~TestEnvironment();

  /// @brief Set up test environment
  virtual void SetUp() override;

  /// @brief Tear-down test environment
  virtual void TearDown() override;

  /// @brief Name of the current test-case
  ///
  /// @return Name of the current test-case or an empty string if called outside a test
  std::string testCaseName() const;

  /// @brief Name of the current test
  ///
  /// @return Name of the current test or an empty string if called outside a test
  std::string testName() const;

  /// @brief Get the ressource path of the unittest
  const platform::Path& getRessourcePath() const { return ressourcePath_; }
  
  /// @brief Get the ressource path of the unittest
  const platform::Path& getTemporaryPath() const { return temporaryPath_; }

  /// @brief Get the render-system to use
  render::RenderSystemKind getRenderSystemKind() const { return renderSystemKind_; }
  
  /// @brief Get a **refrence** to the file specified by `path` relative to the unittest
  /// ressource root (i.e `Environment::getRessourcePath()`)
  ///
  /// @param path   Path relative to the unittest ressource root
  std::shared_ptr<File> getFile(const char* path) const;
  
  /// @brief **Create** a file specified by `path` relative to the unittest temporary root
  /// (i.e `Environment::getRessourcePath()`)
  ///
  /// This function will create all intermediate directories.
  ///
  /// @param path   Path relative to the unittest temporary root
  std::shared_ptr<File> createFile(const char* path) const;

private:
  /// Stack trace
  core::PrettyStackTrace trace_;

  /// Path of the ressources
  platform::Path ressourcePath_;
  
  /// Path to the temporary files
  platform::Path temporaryPath_;

  /// Renderer to use
  render::RenderSystemKind renderSystemKind_;

  /// Manager of the singletons
  std::unique_ptr<core::SingletonManager> singletonManager_;
};

} // namespace unittest

} // namespace sequoia

#endif
