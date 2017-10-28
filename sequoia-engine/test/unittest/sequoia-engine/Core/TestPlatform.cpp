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
#include <gtest/gtest.h>

using namespace sequoia::platform;

namespace {

TEST(PlatformTest, CopyCString) {
  std::string string("string");
  std::wstring wstring(L"wstring");

  auto* copyString = copyCString(string);
  auto* copyWString = copyCString(wstring);

  EXPECT_STREQ(string.c_str(), copyString);
  EXPECT_STREQ(wstring.c_str(), copyWString);

  delete[] copyString;
  delete[] copyWString;
}

TEST(PlatformTest, String) {
  String str(PLATFORM_STR("a string"));
  (void)str;
}

} // anonymous namespace
