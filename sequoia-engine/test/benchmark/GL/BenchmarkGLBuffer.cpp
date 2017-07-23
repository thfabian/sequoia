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

#include "sequoia/Core/AlignedADT.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLBuffer.h"
#include "sequoia/Unittest/BenchmarkMain.h"
#include "sequoia/Unittest/GL/GLBenchmarkEnvironment.h"
#include "sequoia/Unittest/GL/GLRenderSetup.h"

using namespace sequoia;
using namespace sequoia::render;
using namespace sequoia::unittest;

namespace {

class BM_GLBuffer : public BenchmarkFixture<GLRenderSetup> {};

BENCHMARK_DEFINE_F(BM_GLBuffer, DMAWriteDiscardable)(benchmark::State& state) {
  GLenum target = GL_ARRAY_BUFFER;
  std::size_t numBytes = state.range(0);

  // Initialize OpenGL buffer
  GLBuffer buffer(target, 1);
  buffer.allocate(numBytes, Buffer::UH_DynamicWriteOnlyDiscardable);
  core::aligned_vector<Byte> data(numBytes);

  // Bind buffer
  unsigned int id = buffer.getModifyBufferID();
  glBindBuffer(target, id);

  // Benchmark DMA (while discarding the buffer first)
  while(state.KeepRunning()) {
    glBufferData(target, numBytes, nullptr, GL_STREAM_DRAW);
    void* src = glMapBuffer(target, GL_WRITE_ONLY);
    std::memcpy(data.data(), src, numBytes);
    glUnmapBuffer(target);
  }

  glBindBuffer(target, 0);
}
BENCHMARK_REGISTER_F(BM_GLBuffer, DMAWriteDiscardable)
    ->RangeMultiplier(2)
    ->Range(32, 64 /*2 << 12*/);

} // anonymous namespace

SEQUOIA_BENCHMARK_MAIN(sequoia::unittest::GLBenchmarkEnvironment);
