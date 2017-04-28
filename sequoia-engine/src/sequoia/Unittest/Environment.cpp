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

#include "sequoia/Unittest/Environment.h"

namespace sequoia {

SEQUIOA_DECLARE_SINGLETON(unittest::Environment);

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
