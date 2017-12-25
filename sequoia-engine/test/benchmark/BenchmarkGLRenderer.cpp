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

#include "sequoia-engine/Render/GL/GLRenderer.h"
#include "sequoia-engine/Unittest/BenchmarkMain.h"
#include "sequoia-engine/Unittest/GL/GLBenchmarkEnvironment.h"
#include "sequoia-engine/Unittest/RenderSetup.h"

using namespace sequoia::render;
using namespace sequoia::unittest;

namespace {

class BenchmarkGLRenderer : public BenchmarkFixture<RenderSetup> {};

// Benchmark reset of Renderer

BENCHMARK_F(BenchmarkGLRenderer, BM_ResetAll)(benchmark::State& state) {
  GLRenderer* renderer = getGLRendererPtr();
  while(state.KeepRunning()) {
    renderer->reset();
  }
}
BENCHMARK_REGISTER_F(BenchmarkGLRenderer, BM_ResetAll);

} // anonymous namespace

SEQUOIA_BENCHMARK_MAIN(sequoia::unittest::GLBenchmarkEnvironment);
