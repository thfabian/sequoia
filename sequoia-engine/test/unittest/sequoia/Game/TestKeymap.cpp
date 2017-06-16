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

#include "sequoia/Game/Keymap.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::game;
using namespace sequoia::render;

namespace {

TEST(KeymapTest, Key) {
  {
    Keymap map(Key_W);

    EXPECT_EQ(map.getKey(), Key_W);
    EXPECT_EQ(map.getMod(), Mod_NoModifier);
    EXPECT_EQ(map.getButton(), MouseButton_Invalid);

    render::KeyboardEvent eventTrue{nullptr, Key_W, Action_Pressed, 0};
    render::KeyboardEvent eventFalse1{nullptr, Key_S, Action_Pressed, 0};
    render::MouseButtonEvent eventFalse2{nullptr, MouseButton_Left, Action_Pressed, 0};

    EXPECT_TRUE(map.handle(eventTrue));
    EXPECT_FALSE(map.handle(eventFalse1));
    EXPECT_FALSE(map.handle(eventFalse2));
  }

  {
    Keymap map(Key_W, Mod_Alt);

    EXPECT_EQ(map.getKey(), Key_W);
    EXPECT_EQ(map.getMod(), Mod_Alt);
    EXPECT_EQ(map.getButton(), MouseButton_Invalid);

    render::KeyboardEvent eventTrue{nullptr, Key_W, Action_Pressed, Mod_Alt};
    render::KeyboardEvent eventFalse1{nullptr, Key_W, Action_Pressed, Mod_Ctrl};
    render::MouseButtonEvent eventFalse2{nullptr, MouseButton_Left, Action_Pressed, Mod_Alt};

    EXPECT_TRUE(map.handle(eventTrue));
    EXPECT_FALSE(map.handle(eventFalse1));
    EXPECT_FALSE(map.handle(eventFalse2));
  }
}

TEST(KeymapTest, Mouse) {
  {
    Keymap map(MouseButton_Right);

    EXPECT_EQ(map.getKey(), Key_Invalid);
    EXPECT_EQ(map.getMod(), Mod_NoModifier);
    EXPECT_EQ(map.getButton(), MouseButton_Right);

    render::MouseButtonEvent eventTrue{nullptr, MouseButton_Right, Action_Pressed, 0};
    render::MouseButtonEvent eventFalse1{nullptr, MouseButton_Left, Action_Pressed, 0};
    render::KeyboardEvent eventFalse2{nullptr, Key_S, Action_Pressed, 0};

    EXPECT_TRUE(map.handle(eventTrue));
    EXPECT_FALSE(map.handle(eventFalse1));
    EXPECT_FALSE(map.handle(eventFalse2));
  }

  {
    Keymap map(MouseButton_Right, Mod_Alt);

    EXPECT_EQ(map.getKey(), Key_Invalid);
    EXPECT_EQ(map.getMod(), Mod_Alt);
    EXPECT_EQ(map.getButton(), MouseButton_Right);

    render::MouseButtonEvent eventTrue{nullptr, MouseButton_Right, Action_Pressed, Mod_Alt};
    render::MouseButtonEvent eventFalse1{nullptr, MouseButton_Left, Action_Pressed, Mod_Ctrl};
    render::KeyboardEvent eventFalse2{nullptr, Key_S, Action_Pressed, Mod_Alt};

    EXPECT_TRUE(map.handle(eventTrue));
    EXPECT_FALSE(map.handle(eventFalse1));
    EXPECT_FALSE(map.handle(eventFalse2));
  }
}
} // anonymous namespace
