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

class BM_GLBuffer : public BenchmarkFixture<GLRenderSetup> {
protected:
  GLenum target_ = GL_ARRAY_BUFFER;
  std::unique_ptr<GLBuffer> buffer_;
  core::aligned_vector<Byte> data_;

public:
  using Base = BenchmarkFixture<GLRenderSetup>;

  void SetUp(benchmark::State& state) {
    Base::SetUp(state);

    std::size_t numBytes = state.range(0);

    // Initialize OpenGL buffer
    buffer_ = std::make_unique<GLBuffer>(target_, 1);
    buffer_->allocate(numBytes, Buffer::UH_DynamicWriteOnlyDiscardable);
    data_.resize(numBytes);

    // Bind buffer
    buffer_->bind(GLBuffer::BK_Modify);
  }

  void TearDown(benchmark::State& state) {
    buffer_->unbind(GLBuffer::BK_Modify);
    buffer_.release();
    data_.clear();

    Base::TearDown(state);
  }
};

BENCHMARK_DEFINE_F(BM_GLBuffer, DMAWriteDiscard)(benchmark::State& state) {
  const unsigned int numBytes = data_.size();
  const void* src = data_.data();
  const GLenum target = target_;

  while(state.KeepRunning()) {
    /*glBufferData(target, numBytes, nullptr, GL_STREAM_DRAW);*/
    void* dest = glMapBuffer(target, GL_WRITE_ONLY);
    std::memcpy(dest, src, numBytes);
    glUnmapBuffer(target);
  }
}

BENCHMARK_REGISTER_F(BM_GLBuffer, DMAWriteDiscard)->RangeMultiplier(2)->Range(32, /*64*/ 2 << 12);

} // anonymous namespace

SEQUOIA_BENCHMARK_MAIN(sequoia::unittest::GLBenchmarkEnvironment);
