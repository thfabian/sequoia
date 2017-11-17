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

#include "sequoia-engine/Render/VertexFactory.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

TEST(VertexFactoryTest, Create) {
  auto adapter = VertexFactory::create("Vertex_posf3_norf3_texf2_colu4");
  EXPECT_EQ(static_cast<VertexID>(adapter.getLayout().VertexID),
            VertexID::Vertex_posf3_norf3_texf2_colu4);
}

TEST(VertexFactoryTest, Invalid) {
  EXPECT_THROW(VertexFactory::create("unknown"), RenderSystemException);
}

} // anonymous namespace
