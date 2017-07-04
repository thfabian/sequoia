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

#include "sequoia/Core/DoubleBuffered.h"
#include <gtest/gtest.h>

using namespace sequoia;

namespace {

TEST(DoubleBufferedTest, Allocate) {
  DoubleBuffered<std::string> ab("a", "b");
  EXPECT_TRUE(ab.first() == "a");
  EXPECT_TRUE(ab.second() == "b");
  EXPECT_TRUE(ab.get() == "a");

  DoubleBuffered<std::string> empty;
  EXPECT_TRUE(empty.first().empty());
  EXPECT_TRUE(empty.second().empty());

  DoubleBuffered<std::unique_ptr<double>> uptrs(std::make_unique<double>(4.0),
                                                std::make_unique<double>(5.0));
  EXPECT_TRUE(*uptrs.first() == 4.0);
  EXPECT_TRUE(*uptrs.second() == 5.0);
  EXPECT_TRUE(*uptrs.get() == 4.0);
}

TEST(DoubleBufferedTest, Swap) {
  {
    DoubleBuffered<std::string> ab("a", "b");
    EXPECT_TRUE(ab.get() == "a");
    ab.swap(false);
    EXPECT_TRUE(ab.get() == "b");
    ab.swap(false);
    EXPECT_TRUE(ab.get() == "a");
    ab.swap(false);
    EXPECT_TRUE(ab.get() == "b");
  }

  {
    // first: "a"     second: "b"    idx : 0
    DoubleBuffered<std::string> ab("a", "b");
    EXPECT_TRUE(ab.get() == "a");
    std::string& a = ab.get();

    // first: "a"     second: "a"    idx : 1    [swap with copy meaning `second = first`]
    ab.swap();
    EXPECT_TRUE(ab.get() == "a");

    // first: "aaa"   second: "a"    idx : 1    [modify first, second remains the same]
    a = "aaa";
    EXPECT_TRUE(ab.get() == "a");

    // first: "a"     second: "a"    idx : 0    [swap and copy again meaning `first = second`]
    ab.swap();
    EXPECT_TRUE(ab.get() == "a");
    EXPECT_TRUE(a == "a");
  }
}

TEST(DoubleBufferedTest, SwapSharedPtr) {
  // first: "a"     second: "b"    idx : 0
  DoubleBuffered<std::shared_ptr<std::string>> ab(std::make_shared<std::string>("a"),
                                                  std::make_shared<std::string>("b"));
  EXPECT_TRUE(*ab.get() == "a");
  std::string& a = *ab.get();

  // first: "a"     second: "a"    idx : 1    [swap with copy meaning `second = first`]
  ab.swap();
  EXPECT_TRUE(*ab.get() == "a");

  // first: "aaa"   second: "a"    idx : 1    [modify first, second remains the same]
  a = "aaa";
  EXPECT_TRUE(*ab.get() == "a");

  // first: "a"     second: "a"    idx : 0    [swap and copy again meaning `first = second`]
  ab.swap();
  EXPECT_TRUE(*ab.get() == "a");
  EXPECT_TRUE(a == "a");
}

TEST(DoubleBufferedTest, SwapUniquePtr) {
  // first: "a"     second: "b"    idx : 0
  DoubleBuffered<std::unique_ptr<std::string>> ab(std::make_unique<std::string>("a"),
                                                  std::make_unique<std::string>("b"));
  EXPECT_TRUE(*ab.get() == "a");
  std::string& a = *ab.get();

  // first: "a"     second: "a"    idx : 1    [swap with copy meaning `second = first`]
  ab.swap();
  EXPECT_TRUE(*ab.get() == "a");

  // first: "aaa"   second: "a"    idx : 1    [modify first, second remains the same]
  a = "aaa";
  EXPECT_TRUE(*ab.get() == "a");

  // first: "a"     second: "a"    idx : 0    [swap and copy again meaning `first = second`]
  ab.swap();
  EXPECT_TRUE(*ab.get() == "a");
  EXPECT_TRUE(a == "a");
}

} // anonymous namespace
