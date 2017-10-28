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

#ifndef SEQUOIA_ENGINE_UNITTEST_BENCHMARKMAIN_H
#define SEQUOIA_ENGINE_UNITTEST_BENCHMARKMAIN_H

#include <benchmark/benchmark.h>
#include <memory>

/// @brief Define a benchmark main funtion and setup the given `Environment`
///
/// @param Environment    Environment to set-up
/// @ingroup unittest
#define SEQUOIA_BENCHMARK_MAIN(Environment)                                                        \
  int main(int argc, char* argv[]) {                                                               \
    auto env = std::make_unique<Environment>(argc, argv);                                          \
    env->SetUp();                                                                                  \
    benchmark::RunSpecifiedBenchmarks();                                                           \
    env->TearDown();                                                                               \
    return 0;                                                                                      \
  }

#endif
