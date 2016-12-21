//===-- sequoia/Unittest/Environment.cpp --------------------------------------------*- C++ -*-===//
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

#include "sequoia/Unittest/Environment.h"

template <>
sequoia::unittest::Environment* Ogre::Singleton<sequoia::unittest::Environment>::msSingleton =
    nullptr;

namespace sequoia {

namespace unittest {

void Environment::SetUp() {}

void Environment::TearDown() {}

std::string Environment::testCaseName() const {
  const ::testing::TestInfo* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
  if(testInfo)
    return testInfo->test_case_name();
  return "";
}

std::string Environment::testName() const {
  const ::testing::TestInfo* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
  if(testInfo)
    return testInfo->name();
  return "";
}

} // namespace unittest

} // namespace sequoia
