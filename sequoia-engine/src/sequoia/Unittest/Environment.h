//===-- sequoia/Unittest/Environment.h ----------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Setup the global test environment.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_UNITTEST_ENVIRONMENT_H
#define SEQUOIA_UNITTEST_ENVIRONMENT_H

#include "sequoia/Unittest/Export.h"
#include <OGRE/OgreSingleton.h>
#include <gtest/gtest.h>

namespace sequoia {

namespace unittest {

/// @brief Global test environment
class SEQUOIA_UNITTEST_EXPORT Environment : public ::testing::Environment,
                                            public Ogre::Singleton<Environment> {

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
