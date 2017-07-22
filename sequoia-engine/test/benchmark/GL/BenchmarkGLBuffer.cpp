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

#include "sequoia/Render/GL/GLBuffer.h"
#include "sequoia/Unittest/Benchmark.h"
#include "sequoia/Unittest/GL/GLRenderSetup.h"

using namespace sequoia;
using namespace sequoia::render;

namespace {

SEQUOIA_RENDER_BENCHMARK_FIXTURE(BM_GLBuffer)

BENCHMARK_F(BM_GLBuffer, Write)(benchmark::State& st) {
  while(st.KeepRunning()) {
  }
}
BENCHMARK_REGISTER_F(BM_GLBuffer, Write);

} // anonymous namespace

SEQUOIA_BENCHMARK_MAIN();
