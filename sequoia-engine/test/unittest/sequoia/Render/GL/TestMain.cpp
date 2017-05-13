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

#include "sequoia/Unittest/GL/GLEnvironment.h"

#include "sequoia/Core/MicroBenchmark.h"

#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
  // Initialize gtest
  testing::InitGoogleTest(&argc, argv);

  // Register test environment
  testing::AddGlobalTestEnvironment(new sequoia::unittest::GLEnvironment(argc, argv));

  double value = 4.0;
  int n = 1000;
  while(--n) {  
    MIB_START("test2");
    value += std::pow(value, 0.5);
    MIB_STOP("test2");    
  }
  
  std::cout << value << std::endl;
  
  MIB_PRINT("cycle");
  
//  return RUN_ALL_TESTS();
}
