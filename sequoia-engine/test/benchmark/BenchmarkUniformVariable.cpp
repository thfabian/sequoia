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

#include "sequoia/Core/Any.h"
#include "sequoia/Render/UniformVariable.h"
#include <benchmark/benchmark.h>

using namespace sequoia;

// Benchmark boost::any for assigning data

static void BM_UniformVariableAnyMat4(benchmark::State& state) {
  core::any any;
  while(state.KeepRunning())
    benchmark::DoNotOptimize(any = math::mat4(2.0f));
}
BENCHMARK(BM_UniformVariableAnyMat4);

static void BM_UniformVariableAnyInt(benchmark::State& state) {
  core::any any;
  while(state.KeepRunning())
    benchmark::DoNotOptimize(any = int(2));
}
BENCHMARK(BM_UniformVariableAnyInt);

// Benchmark boost::variant for assigning data

static void BM_UniformVariableVariantMat4(benchmark::State& state) {
  UniformVariable::DataType variant;
  while(state.KeepRunning())
    benchmark::DoNotOptimize(variant = math::mat4(2.0f));
}
BENCHMARK(BM_UniformVariableVariantMat4);

static void BM_UniformVariableVariantInt(benchmark::State& state) {
  UniformVariable::DataType variant;
  while(state.KeepRunning())
    benchmark::DoNotOptimize(variant = int(2));
}
BENCHMARK(BM_UniformVariableVariantInt);

BENCHMARK_MAIN();
