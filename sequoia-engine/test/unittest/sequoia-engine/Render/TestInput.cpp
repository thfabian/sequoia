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

#include "sequoia-engine/Render/Input.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace sequoia::render;

namespace {

template <class T>
inline std::string asString(T t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}

TEST(InputTest, KeyboardKey) {
  EXPECT_STREQ(asString(Key_Invalid).c_str(), "Invalid");

  // ASCII keys
  EXPECT_STREQ(asString(Key_5).c_str(), "5");
  EXPECT_STREQ(asString(Key_Equal).c_str(), "=");
  EXPECT_STREQ(asString(Key_X).c_str(), "X");
  EXPECT_STREQ(asString(Key_RightBracket).c_str(), "]");

  // Non-ASCII keys
  EXPECT_STREQ(asString(Key_Backspace).c_str(), "Backspace");
  EXPECT_STREQ(asString(Key_PrintScreen).c_str(), "PrintScreen");
  EXPECT_STREQ(asString(Key_F12).c_str(), "F12");
  EXPECT_STREQ(asString(Key_Menu).c_str(), "Menu");
}

TEST(InputTest, MouseButton) {
  EXPECT_STREQ(asString(MouseButton_Left).c_str(), "Left");
  EXPECT_STREQ(asString(MouseButton_Right).c_str(), "Right");
  EXPECT_STREQ(asString(MouseButton_Middle).c_str(), "Middle");
  EXPECT_STREQ(asString(MouseButton_4).c_str(), "Button 4");
  EXPECT_STREQ(asString(MouseButton_5).c_str(), "Button 5");
  EXPECT_STREQ(asString(MouseButton_6).c_str(), "Button 6");
  EXPECT_STREQ(asString(MouseButton_7).c_str(), "Button 7");
  EXPECT_STREQ(asString(MouseButton_8).c_str(), "Button 8");
}

TEST(InputTest, KeyModifier) {
  EXPECT_STREQ(asString(Mod_NoModifier).c_str(), "NoModifier");
  EXPECT_STREQ(asString(Mod_Shift).c_str(), "Shift");
  EXPECT_STREQ(asString(Mod_Ctrl).c_str(), "Ctrl");
  EXPECT_STREQ(asString(Mod_Alt).c_str(), "Alt");
  EXPECT_STREQ(asString(Mod_Super).c_str(), "Super");
}

} // anonymous namespace
