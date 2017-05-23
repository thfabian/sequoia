//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Casting.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Unittest/Environment.h"
#include "sequoia/Unittest/GL/GLRenderTest.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

class GLVertexArrayObjectTest : public GLRenderTest {};

TEST_F(GLVertexArrayObjectTest, AllocateBuffer) {
  RenderSystem& rsys = RenderSystem::getSingleton();
}

} // anonymous namespace
