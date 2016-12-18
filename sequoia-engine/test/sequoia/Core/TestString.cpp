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

TEST(String, String) {
   sequoia::core::String s(L"Fabian Thüring");

   // To std::wstring
   std::wstring wstring = s;
   EXPECT_EQ(std::wstring(L"Fabian Thüring"), wstring);

   // To std::string
   std::string string = s;
   EXPECT_EQ(std::string("Fabian Thüring"), string);
}


TEST(String, CopyCString) {
  std::string string("string");
  std::wstring wstring(L"wstring");

  auto* copyString = copyCString(string);
  auto* copyWString = copyCString(wstring);

  EXPECT_STREQ(string.c_str(), copyString);
  EXPECT_STREQ(wstring.c_str(), copyWString);

  delete copyString;
  delete copyWString;
}
