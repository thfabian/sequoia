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

#include "sequoia-engine/Render/VertexAdapter.h"
#include "sequoia-engine/Render/VertexFactory.h"
#include <gtest/gtest.h>

using namespace sequoia::render;
using namespace sequoia;

namespace {

TEST(VertexAdapterTest, SetPosition) {
  auto adapter = VertexFactory::create("Vertex_posf3_norf3_texf2_colu4");
  Vertex_posf3_norf3_texf2_colu4 vertex;

  adapter.setPosition(math::vec3(1, 2, 3));
  adapter.copyTo(&vertex);

  EXPECT_EQ(vertex.Position[0], 1);
  EXPECT_EQ(vertex.Position[1], 2);
  EXPECT_EQ(vertex.Position[2], 3);

  adapter.setPosition(math::vec2(3, 2));
  adapter.copyTo(&vertex);

  EXPECT_EQ(vertex.Position[0], 3);
  EXPECT_EQ(vertex.Position[1], 2);
  EXPECT_EQ(vertex.Position[2], 0);

  std::array<float, 4> position{{4, 3, 2, 1}};
  adapter.setPosition(position.data(), position.size());
  adapter.copyTo(&vertex);

  EXPECT_EQ(vertex.Position[0], 4);
  EXPECT_EQ(vertex.Position[1], 3);
  EXPECT_EQ(vertex.Position[2], 2);
}

TEST(VertexAdapterTest, SetNormal) {
  auto adapter = VertexFactory::create("Vertex_posf3_norf3_texf2_colu4");
  Vertex_posf3_norf3_texf2_colu4 vertex;

  adapter.setNormal(math::vec3(1, 2, 3));
  adapter.copyTo(&vertex);

  EXPECT_EQ(vertex.Normal[0], 1);
  EXPECT_EQ(vertex.Normal[1], 2);
  EXPECT_EQ(vertex.Normal[2], 3);

  adapter.setNormal(math::vec2(3, 2));
  adapter.copyTo(&vertex);

  EXPECT_EQ(vertex.Normal[0], 3);
  EXPECT_EQ(vertex.Normal[1], 2);
  EXPECT_EQ(vertex.Normal[2], 0);

  std::array<float, 4> normal{{4, 3, 2, 1}};
  adapter.setNormal(normal.data(), normal.size());
  adapter.copyTo(&vertex);

  EXPECT_EQ(vertex.Normal[0], 4);
  EXPECT_EQ(vertex.Normal[1], 3);
  EXPECT_EQ(vertex.Normal[2], 2);
}

TEST(VertexAdapterTest, SetTexCoord) {
  auto adapter = VertexFactory::create("Vertex_posf3_norf3_texf2_colu4");
  Vertex_posf3_norf3_texf2_colu4 vertex;

  adapter.setTexCoord(math::vec3(1, 2, 3));
  adapter.copyTo(&vertex);

  EXPECT_EQ(vertex.TexCoord[0], 1);
  EXPECT_EQ(vertex.TexCoord[1], 2);

  adapter.setTexCoord(math::vec2(3, 2));
  adapter.copyTo(&vertex);

  EXPECT_EQ(vertex.TexCoord[0], 3);
  EXPECT_EQ(vertex.TexCoord[1], 2);
}

} // anonymous namespace
