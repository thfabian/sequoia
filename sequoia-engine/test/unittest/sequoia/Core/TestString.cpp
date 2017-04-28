//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _       
//                        / ____|                      (_)      
//                       | (___   ___  __ _ _   _  ___  _  __ _ 
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |                    
//                                       |_| 
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/NString.h"
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace {

TEST(StringTest, CopyCString) {
  std::string string("string");
  std::wstring wstring(L"wstring");

  auto copyString = copyCString(string);
  auto copyWString = copyCString(wstring);

  EXPECT_STREQ(string.c_str(), copyString.get());
  EXPECT_STREQ(wstring.c_str(), copyWString.get());
}

} // anonymous namespace
