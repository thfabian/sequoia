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

#include "sequoia/Core/Options2.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia::core;
using namespace sequoia::unittest;

namespace {

TEST(OptionTest, Invalid) {
  Option opt{};
  EXPECT_TRUE(opt.isInvalid());
}

TEST(OptionTest, Bool) {
  Option opt{true};
  EXPECT_TRUE(opt.isOfType<bool>());
  
  EXPECT_EQ(opt.get<bool>(), true);
  EXPECT_EQ(opt.get<int>(), 1);
  EXPECT_EQ(opt.get<float>(), 1.0f);
  EXPECT_STREQ(opt.get<std::string>().c_str(), "1");
}

TEST(OptionTest, Int) {
  Option opt{3};
  EXPECT_TRUE(opt.isOfType<int>());
  
  EXPECT_EQ(opt.get<bool>(), true);
  EXPECT_EQ(opt.get<int>(), 3);
  EXPECT_EQ(opt.get<float>(), 3.0f);
}

TEST(OptionTest, Float) {
  Option opt{3.2f};
  EXPECT_TRUE(opt.isOfType<float>());
  
  EXPECT_EQ(opt.get<bool>(), true);
  EXPECT_EQ(opt.get<int>(), 3);
  EXPECT_EQ(opt.get<float>(), 3.2f);
}

TEST(OptionTest, String) {
  Option opt1{"foo"};
  EXPECT_TRUE(opt1.isOfType<std::string>());

  EXPECT_THROW(opt1.get<bool>(), Exception);
  EXPECT_THROW(opt1.get<int>(), Exception);
  EXPECT_THROW(opt1.get<float>(), Exception);
  EXPECT_STREQ(opt1.get<std::string>().c_str(), "foo");

  Option opt2{"1"};
  EXPECT_EQ(opt2.get<bool>(), true);
  EXPECT_EQ(opt2.get<int>(), 1);
  EXPECT_EQ(opt2.get<float>(), 1.0f);

  Option opt3{"2"};
  EXPECT_THROW(opt3.get<bool>(), Exception);
  EXPECT_EQ(opt3.get<int>(), 2);
  EXPECT_EQ(opt3.get<float>(), 2.0f);

  Option opt4{"2.2"};
  EXPECT_THROW(opt4.get<bool>(), Exception);
  EXPECT_THROW(opt4.get<int>(), Exception);
  EXPECT_EQ(opt4.get<float>(), 2.2f);
}

TEST(OptionsTest, ReadAndWrite) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto file = env.createFile("sequoia/Core/TestOptions/Config.ini");
  
  Options2 options;
  options.set("Foo.Int", 2);
  options.set("Foo.Float", 2.2f);
  options.set("Foo.String", "foo");
  options.write(file->getPath());
}

} // anonymous namespace
