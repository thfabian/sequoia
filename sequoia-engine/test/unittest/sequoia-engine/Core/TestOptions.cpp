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

#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Options2.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia::core;
using namespace sequoia::unittest;

namespace {

TEST(OptionTest, Invalid) {
  Options2 options;
  EXPECT_THROW(options.getAsString("Invalid"), Exception);
}

TEST(OptionTest, Bool) {
  Options2 options;
  options.set("Foo.Bool1", true);

  EXPECT_EQ(options.get<bool>("Foo.Bool1"), true);
  EXPECT_EQ(options.get<int>("Foo.Bool1"), 1);
  EXPECT_EQ(options.get<float>("Foo.Bool1"), 1.0f);
  EXPECT_EQ(options.get<std::string>("Foo.Bool1"), "1");

  options.set("Foo.Bool2", "True");
  EXPECT_EQ(options.get<bool>("Foo.Bool2"), true);
  EXPECT_EQ(options.get<std::string>("Foo.Bool2"), "True");
}

TEST(OptionTest, Int) {
  Options2 options;
  options.set("Foo.Int", 2);

  EXPECT_EQ(options.get<bool>("Foo.Int"), true);
  EXPECT_EQ(options.get<int>("Foo.Int"), 2);
  EXPECT_EQ(options.get<float>("Foo.Int"), 2.0f);
  EXPECT_EQ(options.get<std::string>("Foo.Int"), "2");

  options.set("Foo.Bool", 0);
  EXPECT_EQ(options.get<bool>("Foo.Bool"), false);
}

TEST(OptionTest, Float) {
  Options2 options;
  options.set("Foo.Float", 2.2);

  EXPECT_EQ(options.get<bool>("Foo.Float"), true);
  EXPECT_EQ(options.get<int>("Foo.Float"), 2);
  EXPECT_EQ(options.get<float>("Foo.Float"), 2.2f);

  options.set("Foo.Bool", 0.0f);
  EXPECT_EQ(options.get<bool>("Foo.Bool"), false);
}

TEST(OptionsTest, ReadAndWrite) {
  TestEnvironment& env = TestEnvironment::getSingleton();
  auto file = env.createFile("sequoia/Core/TestOptions/Config.ini");

  Options2 optionsWrite;
  optionsWrite.set("Foo.Int", 2);
  optionsWrite.set("Foo.Float", 2.2f);
  optionsWrite.set("Bar.String", "bar");
  optionsWrite.set("Bar.Bool", false);
  optionsWrite.write(file->getPath());

  Options2 optionsRead;
  optionsRead.read(file->getPath());

  EXPECT_EQ(optionsRead.get<int>("Foo.Int"), optionsWrite.get<int>("Foo.Int"));
  EXPECT_EQ(optionsRead.get<float>("Foo.Float"), optionsWrite.get<float>("Foo.Float"));
  EXPECT_EQ(optionsRead.get<std::string>("Bar.String"),
            optionsWrite.get<std::string>("Bar.String"));
  EXPECT_EQ(optionsRead.get<bool>("Bar.Bool"), optionsWrite.get<bool>("Bar.Bool"));
}

TEST(OptionsTest, CopyConstructor) {
  Options2 options;
  options.set("Foo.Float", 2.2);
  options.set("Foo.Int", 2);

  Options2 optionsCopy = options.getCopy();

  options.set("Foo.Int", 3);
  options.set("Foo.Float", 3.2);

  EXPECT_EQ(optionsCopy.get<int>("Foo.Int"), 2);
  EXPECT_EQ(optionsCopy.get<float>("Foo.Float"), 2.2f);
  EXPECT_EQ(options.get<int>("Foo.Int"), 3);
  EXPECT_EQ(options.get<float>("Foo.Float"), 3.2f);
}

} // anonymous namespace