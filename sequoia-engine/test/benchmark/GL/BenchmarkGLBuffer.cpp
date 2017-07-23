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

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLBuffer.h"
#include "sequoia/Unittest/BenchmarkMain.h"
#include "sequoia/Unittest/GL/GLBenchmarkEnvironment.h"
#include "sequoia/Unittest/GL/GLRenderSetup.h"

using namespace sequoia;
using namespace sequoia::render;

namespace {

SEQUOIA_RENDER_BENCHMARK_FIXTURE(BM_GLBuffer)

BENCHMARK_DEFINE_F(BM_GLBuffer, DMAWriteDiscardable)(benchmark::State& state) {
  GLBuffer buffer(GL_ARRAY_BUFFER, 1);
  buffer.allocate(state.range(0), Buffer::UH_DynamicWriteOnlyDiscardable);

  unsigned int id = buffer.getModifyBufferIdx();
  glBindBuffer(GL_ARRAY_BUFFER, id);
  while(state.KeepRunning()) {
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
BENCHMARK_REGISTER_F(BM_GLBuffer, DMAWriteDiscardable)
    ->RangeMultiplier(2)
    ->Range(32, 64 /*2 << 12*/);

} // anonymous namespace

SEQUOIA_BENCHMARK_MAIN(sequoia::unittest::GLBenchmarkEnvironment);
