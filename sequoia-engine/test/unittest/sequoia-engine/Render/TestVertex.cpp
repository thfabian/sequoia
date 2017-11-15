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
#include "sequoia-engine/Render/Vertex2.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

// clang-format off

SEQUOIA_VERTEX(TestVertex, 
               (float, Position, 3, false)
               (float, Normal, 3, false)
               (float, TexCoord, 2, false)
               (std::uint8_t, Color, 4, true));

// clang-format on

TEST(VertexLayoutTest, Test) {
  VertexLayout2 layout = TestVertex::getLayout();
  EXPECT_EQ(layout.SizeOf, sizeof(TestVertex));

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
}

TEST(VertexTest, Vertex2DLayout) {
  EXPECT_EQ(Vertex2D::getLayout()->SizeOf, sizeof(Vertex2D));

  // float Position[2];
  EXPECT_EQ(Vertex2D::getLayout()->Position.Type, VertexLayout::Float32);
  EXPECT_EQ(Vertex2D::getLayout()->Position.NumElements, 2);
  EXPECT_EQ(Vertex2D::getLayout()->Position.Offset, 0);

  // float TexCoord[2];
  EXPECT_EQ(Vertex2D::getLayout()->TexCoord.Type, VertexLayout::Float32);
  EXPECT_EQ(Vertex2D::getLayout()->TexCoord.NumElements, 2);
  EXPECT_EQ(Vertex2D::getLayout()->TexCoord.Offset, 8);

  // unsigned char Color[4];
  EXPECT_EQ(Vertex2D::getLayout()->Color.Type, VertexLayout::UInt8);
  EXPECT_EQ(Vertex2D::getLayout()->Color.NumElements, 4);
  EXPECT_EQ(Vertex2D::getLayout()->Color.Offset, 16);
}

TEST(VertexTest, Vertex3DLayout) {
  EXPECT_EQ(Vertex3D::getLayout()->SizeOf, sizeof(Vertex3D));

  // float Position[3];
  EXPECT_EQ(Vertex3D::getLayout()->Position.Type, VertexLayout::Float32);
  EXPECT_EQ(Vertex3D::getLayout()->Position.NumElements, 3);
  EXPECT_EQ(Vertex3D::getLayout()->Position.Offset, 0);

  // float Normal[3];
  EXPECT_EQ(Vertex3D::getLayout()->Normal.Type, VertexLayout::Float32);
  EXPECT_EQ(Vertex3D::getLayout()->Normal.NumElements, 3);
  EXPECT_EQ(Vertex3D::getLayout()->Normal.Offset, 12);

  // float TexCoord[2];
  EXPECT_EQ(Vertex3D::getLayout()->TexCoord.Type, VertexLayout::Float32);
  EXPECT_EQ(Vertex3D::getLayout()->TexCoord.NumElements, 2);
  EXPECT_EQ(Vertex3D::getLayout()->TexCoord.Offset, 24);

  // unsigned char Color[4];
  EXPECT_EQ(Vertex3D::getLayout()->Color.Type, VertexLayout::UInt8);
  EXPECT_EQ(Vertex3D::getLayout()->Color.NumElements, 4);
  EXPECT_EQ(Vertex3D::getLayout()->Color.Offset, 32);
}


} // anonymous namespace
