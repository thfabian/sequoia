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

#include "sequoia/Render/Vertex.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

TEST(VertexTest, Vertex2DLayout) {
  EXPECT_EQ(Vertex2D::getLayout()->SizeOf, sizeof(Vertex2D));

  // float Position[2];
  EXPECT_EQ(Vertex2D::getLayout()->PositionType, VertexLayout::Float);
  EXPECT_EQ(Vertex2D::getLayout()->PositionNumElement, 2);
  EXPECT_EQ(Vertex2D::getLayout()->PositionOffset, 0);

  // float TexCoord[2];
  EXPECT_EQ(Vertex2D::getLayout()->TexCoordType, VertexLayout::Float);
  EXPECT_EQ(Vertex2D::getLayout()->TexCoordNumElement, 2);
  EXPECT_EQ(Vertex2D::getLayout()->TexCoordOffset, 8);

  // unsigned char Color[4];
  EXPECT_EQ(Vertex2D::getLayout()->ColorType, VertexLayout::UnsignedByte);
  EXPECT_EQ(Vertex2D::getLayout()->ColorNumElement, 4);
  EXPECT_EQ(Vertex2D::getLayout()->ColorOffset, 16);
}

TEST(VertexTest, Vertex3DLayout) {
  EXPECT_EQ(Vertex3D::getLayout()->SizeOf, sizeof(Vertex3D));

  // float Position[3];
  EXPECT_EQ(Vertex3D::getLayout()->PositionType, VertexLayout::Float);
  EXPECT_EQ(Vertex3D::getLayout()->PositionNumElement, 3);
  EXPECT_EQ(Vertex3D::getLayout()->PositionOffset, 0);

  // float Normal[3];
  EXPECT_EQ(Vertex3D::getLayout()->NormalType, VertexLayout::Float);
  EXPECT_EQ(Vertex3D::getLayout()->NormalNumElement, 3);
  EXPECT_EQ(Vertex3D::getLayout()->NormalOffset, 12);

  // float TexCoord[2];
  EXPECT_EQ(Vertex3D::getLayout()->TexCoordType, VertexLayout::Float);
  EXPECT_EQ(Vertex3D::getLayout()->TexCoordNumElement, 2);
  EXPECT_EQ(Vertex3D::getLayout()->TexCoordOffset, 24);

  // unsigned char Color[4];
  EXPECT_EQ(Vertex3D::getLayout()->ColorType, VertexLayout::Float);
  EXPECT_EQ(Vertex3D::getLayout()->ColorNumElement, 3);
  EXPECT_EQ(Vertex3D::getLayout()->ColorOffset, 32);
}

} // anonymous namespace
