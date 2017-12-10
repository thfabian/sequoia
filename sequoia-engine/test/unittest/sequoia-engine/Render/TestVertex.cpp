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

#include "sequoia-engine/Render/Vertex.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

TEST(VertexLayoutTest, Vertex_posf3_norf3_texf2_colu4) {
  VertexLayout layout = Vertex_posf3_norf3_texf2_colu4::getLayout();
  EXPECT_EQ(layout.SizeOf, sizeof(Vertex_posf3_norf3_texf2_colu4));

  // float Position[3];
  EXPECT_EQ(layout.Position.Type, VertexLayout::Float32);
  EXPECT_EQ(layout.Position.NumElements, 3);
  EXPECT_EQ(layout.Position.Offset, 0);
  EXPECT_EQ(layout.Position.Normalize, false);

  // float Normal[3];
  EXPECT_EQ(layout.Normal.Type, VertexLayout::Float32);
  EXPECT_EQ(layout.Normal.NumElements, 3);
  EXPECT_EQ(layout.Normal.Offset, 12);
  EXPECT_EQ(layout.Normal.Normalize, false);

  // float TexCoord[2];
  EXPECT_EQ(layout.TexCoord.Type, VertexLayout::Float32);
  EXPECT_EQ(layout.TexCoord.NumElements, 2);
  EXPECT_EQ(layout.TexCoord.Offset, 24);
  EXPECT_EQ(layout.TexCoord.Normalize, false);

  // unsigned char Color[4];
  EXPECT_EQ(layout.Color.Type, VertexLayout::UInt8);
  EXPECT_EQ(layout.Color.NumElements, 4);
  EXPECT_EQ(layout.Color.Offset, 32);
  EXPECT_EQ(layout.Color.Normalize, true);

  EXPECT_STREQ(layout.getName(), "Vertex_posf3_norf3_texf2_colu4");
}

TEST(VertexLayoutTest, Vertex_posf2_texf2_colu4) {
  VertexLayout layout = Vertex_posf2_texf2_colu4::getLayout();
  EXPECT_EQ(layout.SizeOf, sizeof(Vertex_posf2_texf2_colu4));

  // float Position[2];
  EXPECT_EQ(layout.Position.Type, VertexLayout::Float32);
  EXPECT_EQ(layout.Position.NumElements, 2);
  EXPECT_EQ(layout.Position.Offset, 0);
  EXPECT_EQ(layout.Position.Normalize, false);

  // float TexCoord[2];
  EXPECT_EQ(layout.TexCoord.Type, VertexLayout::Float32);
  EXPECT_EQ(layout.TexCoord.NumElements, 2);
  EXPECT_EQ(layout.TexCoord.Offset, 8);
  EXPECT_EQ(layout.TexCoord.Normalize, false);

  // unsigned char Color[4];
  EXPECT_EQ(layout.Color.Type, VertexLayout::UInt8);
  EXPECT_EQ(layout.Color.NumElements, 4);
  EXPECT_EQ(layout.Color.Offset, 16);
  EXPECT_EQ(layout.Color.Normalize, true);

  EXPECT_STREQ(layout.getName(), "Vertex_posf2_texf2_colu4");
}

} // anonymous namespace
