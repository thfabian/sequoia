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

#include "sequoia-engine/Core/Platform.h"
#include "sequoia-engine/Core/RealFileSystem.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>
#include <memory>

using namespace sequoia::core;
using namespace sequoia::unittest;
using namespace sequoia::platform;

namespace {

class RealFileSystemTest : public testing::Test {
protected:
  std::unique_ptr<RealFileSystem> fs;

  virtual void SetUp() override {
    auto& env = TestEnvironment::getSingleton();
    fs =
        std::make_unique<RealFileSystem>(toAnsiString(env.createTemporaryDir(getCurrentBaseDir())));
  }

  virtual void TearDown() override { fs.reset(); }

  Path getCurrentBaseDir() {
    auto& env = TestEnvironment::getSingleton();
    return Path(PLATFORM_STR("sequoia-engine")) / PLATFORM_STR("Core") /
           asPath(env.testCaseName()) / asPath(env.testName());
  }
};

TEST_F(RealFileSystemTest, Read) {
  auto& env = TestEnvironment::getSingleton();

  env.createTemporaryFile(getCurrentBaseDir() / PLATFORM_STR("foo") / PLATFORM_STR("bar.txt"),
                          "Hello bar!");
  EXPECT_TRUE(fs->exists("foo/bar.txt"));

  env.createTemporaryFile(getCurrentBaseDir() / PLATFORM_STR("foo.txt"), "Hello foo!");
  EXPECT_TRUE(fs->exists("foo.txt"));

  // Read new file
  auto bar = fs->read("foo/bar.txt", FileBuffer::FF_Text);
  ASSERT_TRUE(bar != nullptr);
  EXPECT_STREQ(bar->getDataAsString().c_str(), "Hello bar!");

  // Read same file (should return the same buffer)
  EXPECT_EQ(bar.get(), fs->read("foo/bar.txt", FileBuffer::FF_Text).get());

  // Read another file
  auto foo = fs->read("foo.txt", FileBuffer::FF_Text);
  ASSERT_TRUE(foo != nullptr);
  EXPECT_STREQ(foo->getDataAsString().c_str(), "Hello foo!");
}

TEST_F(RealFileSystemTest, Write) {}

TEST_F(RealFileSystemTest, Add) {}


} // anonymous namespace
