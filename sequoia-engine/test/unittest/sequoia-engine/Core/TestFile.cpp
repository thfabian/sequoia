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

#include "sequoia-engine/Core/File.h"
#include <gtest/gtest.h>

using namespace sequoia::core;

namespace {

TEST(FileTest, Extension) {
  EXPECT_EQ(File::TypeFromExtension(".txt"), FileType::Text);
  EXPECT_EQ(File::TypeFromExtension(".jpeg"), FileType::Jpeg);
  EXPECT_EQ(File::TypeFromExtension("bogus"), FileType::Unknown);
}

TEST(FileTest, ToString) {
  EXPECT_STREQ(File::TypeToString(FileType::Png), "Png");
  EXPECT_STREQ(File::TypeToString(FileType::Bmp), "Bmp");
}

} // anonymous namespace
