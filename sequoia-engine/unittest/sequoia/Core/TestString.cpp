//===--------------------------------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/String.h"
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace {

TEST(StringTest, String) {
  // Narrow strings
  {
    sequoia::String s(L"string");

    std::wstring wstring = s;
    EXPECT_EQ(std::wstring(L"string"), wstring);

    std::string string = s;
    EXPECT_EQ(std::string("string"), string);
  }

#ifdef SEQUOIA_ON_WIN32
  // Wide strings
  {
    sequoia::String s(L"Fabian Thüring");

    // To std::wstring
    std::wstring wstring = s;
    EXPECT_EQ(std::wstring(L"Fabian Thüring"), wstring);

    // To std::string
    std::string string = s;
    EXPECT_EQ(std::string("Fabian Thüring"), string);
  }
#endif
}

TEST(StringTest, CopyCString) {
  std::string string("string");
  std::wstring wstring(L"wstring");

  auto* copyString = copyCString(string);
  auto* copyWString = copyCString(wstring);

  EXPECT_STREQ(string.c_str(), copyString);
  EXPECT_STREQ(wstring.c_str(), copyWString);

  delete copyString;
  delete copyWString;
}

} // anonymous namespace
