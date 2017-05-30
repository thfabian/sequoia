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

#include "sequoia/Render/VertexData.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

TEST(VertexDataTest, WriteAndRead) {
  VertexData data(Vertex3D::getLayout(), 4, 0, true);

  std::size_t numVertices = data.getNumVertices();

  data.write([&numVertices](Vertex3D* vertices) {
    for(int i = 0; i < numVertices; ++i) {
      vertices[i].Position[0] = i;
      vertices[i].Normal[0] = i;
      vertices[i].TexCoord[0] = i;
      vertices[i].Color[0] = i;
    }
  });

  data.read([&numVertices](const Vertex3D* vertices) {
    for(int i = 0; i < numVertices; ++i) {
      EXPECT_EQ(vertices[i].Position[0], i) << "vertex index " << i;
      EXPECT_EQ(vertices[i].Normal[0], i) << "vertex index " << i;
      EXPECT_EQ(vertices[i].TexCoord[0], i) << "vertex index " << i;
      EXPECT_EQ(vertices[i].Color[0], i) << "vertex index " << i;
    }
  });
}

} // anonymous namespace
