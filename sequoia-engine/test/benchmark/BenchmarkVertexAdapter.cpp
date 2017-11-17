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

#include "sequoia-engine/Render/VertexAdapter.h"
#include "sequoia-engine/Render/VertexFactory.h"
#include "sequoia-engine/Unittest/BenchmarkEnvironment.h"
#include "sequoia-engine/Unittest/BenchmarkMain.h"

using namespace sequoia;
using namespace sequoia::render;

namespace {

// Benchmark direct assignment

static void BM_AssignDirect(benchmark::State& state) {
  Vertex_posf3_norf3_texf2_colu4 vertex;
  while(state.KeepRunning()) {
    benchmark::DoNotOptimize(vertex.Position[0] = 1.0f);
    benchmark::DoNotOptimize(vertex.Position[1] = 2.0f);
    benchmark::DoNotOptimize(vertex.Position[2] = 3.0f);
  }
}
BENCHMARK(BM_AssignDirect);

// Benchmark assignment via VertexAdapter

static void BM_AssignAdapter(benchmark::State& state) {
  auto adapter = VertexAdapter(Vertex_posf3_norf3_texf2_colu4::getLayout());
  while(state.KeepRunning()) {
    adapter.setPosition(math::vec3(1.0f, 2.0f, 3.0f));
  }
}
BENCHMARK(BM_AssignAdapter);

} // anonymous namespace

SEQUOIA_BENCHMARK_MAIN(sequoia::unittest::BenchmarkEnvironment);
