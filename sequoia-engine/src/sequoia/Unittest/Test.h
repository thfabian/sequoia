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

#ifndef SEQUOIA_UNITTEST_TEST_H
#define SEQUOIA_UNITTEST_TEST_H

#include "sequoia/Math/Math.h"
#include "sequoia/Core/Export.h"
#include <gtest/gtest.h>

namespace sequoia {

namespace unittest {

namespace internal {

/// @brief Test if `abs(expected - actual) < absErr`
SEQUOIA_API std::pair<bool, std::string> compareHelper(float expected, float actual,
                                                                float absErr);

/// @brief Test if `abs(expected - actual) < absErr` for each element
SEQUOIA_API std::pair<bool, std::string>
compareHelper(const math::vec3& expected, const math::vec3& actual, float absErr,
              const char* expectedStr, const char* actualStr);

} // namespace internal

/// @brief Test if vec3 `expected` is near `actual`
///
/// This effectivly tests `abs(expected - actual) < absErr` for each element.
///
/// @ingroup unittest
#define EXPECT_VEC3_NEAR(expected, actual, absErr)                                                 \
  {                                                                                                \
    auto __sequoia_test__ =                                                                        \
        sequoia::unittest::internal::compareHelper(expected, actual, absErr, #expected, #actual);  \
    EXPECT_TRUE((__sequoia_test__.first)) << __sequoia_test__.second;                              \
  }

} // namespace unittest

} // namespace sequoia

#endif
