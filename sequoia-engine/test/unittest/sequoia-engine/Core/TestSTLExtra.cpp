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

#include "sequoia-engine/Core/STLExtras.h"
#include <gtest/gtest.h>

using namespace sequoia;

namespace {

TEST(STLExtraTest, functional) {
  auto foo1 = [](float, int) -> void {};
  auto foo2 = []() -> double { return double(5.0); };

  static_assert(std::is_same<void, core::function_return_t<decltype(foo1), float, int>>::value, "");
  static_assert(std::is_same<double, core::function_return_t<decltype(foo2)>>::value, "");

  static_assert(std::is_same<float, core::function_first_argument_t<decltype(foo1)>>::value, "");
}

} // anonymous namespace
