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

#include "sequoia/Core/Hash.h"
#include <gtest/gtest.h>

using namespace sequoia;

namespace {

struct Foo {
  int MyInteger;
  float MyFloat;
};

struct Bar {
  Foo MyFoo;
};

} // anonymous namespace

SEQUOIA_DECLARE_STD_HASH(Foo, value, value.MyInteger, value.MyFloat)
SEQUOIA_DECLARE_STD_HASH(Bar, value, value.MyFoo)

namespace {

TEST(HashTest, Hash) {
  // The not equal tests could theoretically fail if we have a collision (very unlikely though)

  EXPECT_EQ(core::hash(1), core::hash(1));
  EXPECT_NE(core::hash(1), core::hash(2));

  EXPECT_EQ(core::hash(Foo{1, 2.0f}), core::hash(Foo{1, 2.0f}));
  EXPECT_NE(core::hash(Foo{1, 2.0f}), core::hash(Foo{1, 1.0f}));

  EXPECT_EQ(core::hash(Bar{Foo{1, 2.0f}}), core::hash(Bar{Foo{1, 2.0f}}));
  EXPECT_NE(core::hash(Bar{Foo{1, 2.0f}}), core::hash(Bar{Foo{1, 1.0f}}));

  EXPECT_EQ(core::hash(1, 2, 3, 4), core::hash(1, 2, 3, 4));
  EXPECT_NE(core::hash(1, 2, 3, 4), core::hash(4, 3, 2, 1));
}

} // anonymous namespace
