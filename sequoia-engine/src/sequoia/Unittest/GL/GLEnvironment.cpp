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

#include "sequoia/Core/Options.h"
#include "sequoia/Unittest/GL/GLEnvironment.h"

namespace sequoia {

namespace unittest {

GLEnvironment::GLEnvironment(int argc, char* argv[]) : Environment(argc, argv) {}

void GLEnvironment::SetUp() {
  renderSystem_ = render::RenderSystem::create(render::RK_OpenGL);
  renderSystem_->setDebugMode(Options::getSingleton().Core.Debug);
}

void GLEnvironment::TearDown() { renderSystem_.reset(); }

} // namespace unittest

} // namespace sequoia
