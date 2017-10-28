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

#include "sequoia-engine/Core/Any.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include "sequoia-engine/Unittest/BenchmarkEnvironment.h"
#include "sequoia-engine/Unittest/BenchmarkMain.h"

using namespace sequoia;

namespace {

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

} // anonymous namespace

SEQUOIA_BENCHMARK_MAIN(sequoia::unittest::BenchmarkEnvironment);
