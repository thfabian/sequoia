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

#include "sequoia-engine/Unittest/GL/GLBenchmarkEnvironment.h"
#include "sequoia-engine/Unittest/TestOptions.h"

namespace sequoia {

namespace unittest {

GLBenchmarkEnvironment::GLBenchmarkEnvironment(int argc, char* argv[])
    : BenchmarkEnvironment(argc, argv) {}

void GLBenchmarkEnvironment::SetUp() {
  renderSystem_ = render::RenderSystem::create(render::RK_OpenGL, TestOptions::getSingletonPtr());
}

void GLBenchmarkEnvironment::TearDown() { renderSystem_.reset(); }

} // namespace unittest

} // namespace sequoia
