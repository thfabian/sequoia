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

#include "sequoia/Render/Vertex.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

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
