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

#ifndef SEQUOIA_ENGINE_UNITTEST_TEST_H
#define SEQUOIA_ENGINE_UNITTEST_TEST_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Math/Math.h"
#include <gtest/gtest.h>

namespace sequoia {

namespace unittest {

namespace internal {

/// @brief Test if `abs(expected - actual) < absErr`
SEQUOIA_API std::pair<bool, std::string> compareHelper(float expected, float actual, float absErr);

/// @brief Test if `abs(expected - actual) < absErr` for each element
SEQUOIA_API std::pair<bool, std::string> compareHelper(const float* expected, const float* actual,
                                                       std::size_t size, float absErr,
                                                       const char* expectedStr,
                                                       const char* actualStr);

} // namespace internal

/// @brief Test if vec `expected` is near `actual`
///
/// This effectivly tests `abs(expected - actual) < absErr` for each element.
///
/// @ingroup unittest
#define EXPECT_VEC_NEAR(expected, actual, absErr)                                                  \
  {                                                                                                \
    EXPECT_EQ(expected.length(), actual.length())                                                  \
        << "\"" << #expected << "\" size " << expected.length() << " not equal to \"" << #actual   \
        << "\" size " << actual.length();                                                          \
    auto __sequoia_test__ = sequoia::unittest::internal::compareHelper(                            \
        math::value_ptr(expected), math::value_ptr(actual), expected.length(), absErr, #expected,  \
        #actual);                                                                                  \
    EXPECT_TRUE((__sequoia_test__.first)) << __sequoia_test__.second;                              \
  }

} // namespace unittest

} // namespace sequoia

#endif
