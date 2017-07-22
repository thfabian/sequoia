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

#ifndef SEQUOIA_UNITTEST_BENCHMARK_H
#define SEQUOIA_UNITTEST_BENCHMARK_H

#include "sequoia/Unittest/Environment.h"
#include <benchmark/benchmark.h>

/// @brief Define a benchmark main funtion
/// @ingroup unittest
#define SEQUOIA_BENCHMARK_MAIN()                                                                   \
  int main(int argc, char* argv[]) {                                                               \
    benchmark::Initialize(&argc, argv);                                                            \
    if(benchmark::ReportUnrecognizedArguments(argc, argv))                                         \
      return 1;                                                                                    \
    benchmark::RunSpecifiedBenchmarks();                                                           \
    return 0;                                                                                      \
  }

#endif
