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

#ifndef SEQUOIA_UNITTEST_ENVIRONMENT_H
#define SEQUOIA_UNITTEST_ENVIRONMENT_H

#include "sequoia/Core/Singleton.h"
#include "sequoia/Unittest/Export.h"
#include <gtest/gtest.h>

namespace sequoia {

namespace unittest {

/// @brief Global test environment
/// @ingroup unittest
class SEQUOIA_UNITTEST_API Environment : public ::testing::Environment,
                                         public Singleton<Environment> {

  /// @brief Set up test environment
  virtual void SetUp() override;

  /// @brief Tear-down test environment
  virtual void TearDown() override;

  /// @brief Name of the current test-case
  ///
  /// @return Name of the current test-case or an empty string if called outside a test
  std::string testCaseName() const;

  /// @brief Name of the current test
  ///
  /// @return Name of the current test or an empty string if called outside a test
  std::string testName() const;
};

} // namespace unittest

} // namespace sequoia

#endif
