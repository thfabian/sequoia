//===--------------------------------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Unittest/Environment.h"
#include <gtest/gtest.h>

int main(int argc, char* argv[]) {

  // Initialize gtest
  testing::InitGoogleTest(&argc, argv);
  
  // Register test environment  
  testing::AddGlobalTestEnvironment(new sequoia::unittest::Environment());
  
  return RUN_ALL_TESTS();
}
