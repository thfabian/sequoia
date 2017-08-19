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

#include "sequoia/Core/Casting.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Render/GL/GLVertexData.h"
#include "sequoia/Unittest/GL/GLRenderSetup.h"
#include "sequoia/Unittest/TestEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

GLenum getGLType(VertexLayout::Type type) {
  switch(type) {
  case VertexLayout::Invalid:
    return GL_INVALID_ENUM;
  case VertexLayout::UnsignedByte:
    return GL_UNSIGNED_BYTE;
  case VertexLayout::Float:
    return GL_FLOAT;
  default:
    sequoia_unreachable("invalid type");
  }
}

template <typename T>
class GLVertexDataTest : public TestFixture<GLRenderSetup> {};

using VertexTypes = testing::Types<Vertex2D, Vertex3D>;
TYPED_TEST_CASE(GLVertexDataTest, VertexTypes);

template <class VertexDataType>
std::shared_ptr<GLVertexData> makeVertexData(std::size_t numVertices, std::size_t numIndices,
                                             bool useVertexShadowBuffer = true,
                                             bool useIndexShadowBuffer = true) {
  RenderSystem& rsys = RenderSystem::getSingleton();
  VertexDataParameter param(render::VertexData::DM_Triangles, VertexDataType::getLayout(),
                            numVertices, numIndices, Buffer::UH_StaticWriteOnly);
  param.UseVertexShadowBuffer = useVertexShadowBuffer;
  param.UseIndexShadowBuffer = useIndexShadowBuffer;
  return dyn_pointer_cast<GLVertexData>(rsys.createVertexData(param));
}

TYPED_TEST(GLVertexDataTest, Allocation) {
  std::shared_ptr<GLVertexData> gldata1 = makeVertexData<TypeParam>(32, 16);
  EXPECT_EQ(gldata1->getNumIndices(), 16);
  EXPECT_EQ(gldata1->getNumVertices(), 32);
  EXPECT_TRUE(gldata1->hasIndices());

  std::shared_ptr<GLVertexData> gldata2 = makeVertexData<TypeParam>(16, 0);
  EXPECT_EQ(gldata2->getNumIndices(), 0);
  EXPECT_EQ(gldata2->getNumVertices(), 16);
  EXPECT_FALSE(gldata2->hasIndices());
}

TYPED_TEST(GLVertexDataTest, VertexAttributes) {
  std::shared_ptr<GLVertexData> gldata = makeVertexData<TypeParam>(8, 8);
  const VertexLayout* layout = gldata->getVertexBuffer()->getLayout();

  gldata->bindForModify();

  // Check Position attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_EQ(enabled, layout->hasPosition());

    if(layout->hasPosition()) {

      GLint bufferID;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &bufferID);
      EXPECT_EQ(bufferID, gldata->getVAOID());

      GLenum type;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
      EXPECT_EQ(type, getGLType(layout->PositionType));

      GLint normalized;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                          &normalized);
      EXPECT_EQ(normalized, layout->PositionNormalized);

      GLint size;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
      EXPECT_EQ(size, layout->PositionNumElement);

      GLint stride;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
      EXPECT_EQ(stride, layout->SizeOf);

      void* pointer;
      glGetVertexAttribPointerv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_POINTER,
                                &pointer);
      EXPECT_EQ((std::size_t)pointer, layout->PositionOffset);
    }
  }

  // Check Normal attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_EQ(enabled, layout->hasNormal());

    if(layout->hasNormal()) {

      GLint bufferID;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &bufferID);
      EXPECT_EQ(bufferID, gldata->getVAOID());

      GLenum type;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
      EXPECT_EQ(type, getGLType(layout->NormalType));

      GLint normalized;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                          &normalized);
      EXPECT_EQ(normalized, layout->NormalNormalized);

      GLint size;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
      EXPECT_EQ(size, layout->NormalNumElement);

      GLint stride;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
      EXPECT_EQ(stride, layout->SizeOf);

      void* pointer;
      glGetVertexAttribPointerv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_POINTER,
                                &pointer);
      EXPECT_EQ((std::size_t)pointer, layout->NormalOffset);
    }
  }

  // Check TexCoord attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_EQ(enabled, layout->hasTexCoord());

    if(layout->hasTexCoord()) {

      GLint bufferID;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &bufferID);
      EXPECT_EQ(bufferID, gldata->getVAOID());

      GLenum type;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
      EXPECT_EQ(type, getGLType(layout->TexCoordType));

      GLint normalized;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                          &normalized);
      EXPECT_EQ(normalized, layout->TexCoordNormalized);

      GLint size;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
      EXPECT_EQ(size, layout->TexCoordNumElement);

      GLint stride;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
      EXPECT_EQ(stride, layout->SizeOf);

      void* pointer;
      glGetVertexAttribPointerv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_POINTER,
                                &pointer);
      EXPECT_EQ((std::size_t)pointer, layout->TexCoordOffset);
    }
  }

  // Check Color attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_EQ(enabled, layout->hasColor());

    if(layout->hasColor()) {

      GLint bufferID;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &bufferID);
      EXPECT_EQ(bufferID, gldata->getVAOID());

      GLenum type;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
      EXPECT_EQ(type, getGLType(layout->ColorType));

      GLint normalized;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized);
      EXPECT_EQ(normalized, layout->ColorNormalized);

      GLint size;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
      EXPECT_EQ(size, layout->ColorNumElement);

      GLint stride;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
      EXPECT_EQ(stride, layout->SizeOf);

      void* pointer;
      glGetVertexAttribPointerv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);
      EXPECT_EQ((std::size_t)pointer, layout->ColorOffset);
    }
  }
}

class TestWriteVisitor : public VertexVisitor {
public:
  void visit(const Vertex3DLayout* layout) {
    Vertex3D* vertices = getVerticesPtr(layout);

    for(int i = 0; i < getNumVertices(); ++i) {
      Vertex3D& vertex = vertices[i];

      for(int j = 0; j < layout->PositionNumElement; ++j)
        vertex.Position[j] = j;

      for(int j = 0; j < layout->NormalNumElement; ++j)
        vertex.Normal[j] = j;

      for(int j = 0; j < layout->TexCoordNumElement; ++j)
        vertex.TexCoord[j] = j;

      for(int j = 0; j < layout->ColorNumElement; ++j)
        vertex.Color[j] = j;
    }
  }

  void visit(const Vertex2DLayout* layout) {
    Vertex2D* vertices = getVerticesPtr(layout);

    for(int i = 0; i < getNumVertices(); ++i) {
      Vertex2D& vertex = vertices[i];

      for(int j = 0; j < layout->PositionNumElement; ++j)
        vertex.Position[j] = j;

      for(int j = 0; j < layout->TexCoordNumElement; ++j)
        vertex.TexCoord[j] = j;

      for(int j = 0; j < layout->ColorNumElement; ++j)
        vertex.Color[j] = j;
    }
  }
};

class TestReadVisitor : public VertexVisitor {
public:
  void visit(const Vertex3DLayout* layout) {
    Vertex3D* vertices = getVerticesPtr(layout);

    for(int i = 0; i < getNumVertices(); ++i) {
      Vertex3D& vertex = vertices[i];

      for(int j = 0; j < layout->PositionNumElement; ++j)
        EXPECT_EQ(vertex.Position[j], j) << "vertex index " << i;

      for(int j = 0; j < layout->NormalNumElement; ++j)
        EXPECT_EQ(vertex.Normal[j], j) << "vertex index " << i;

      for(int j = 0; j < layout->TexCoordNumElement; ++j)
        EXPECT_EQ(vertex.TexCoord[j], j) << "vertex index " << i;

      for(int j = 0; j < layout->ColorNumElement; ++j)
        EXPECT_EQ(vertex.Color[j], j) << "vertex index " << i;
    }
  }

  void visit(const Vertex2DLayout* layout) {
    Vertex2D* vertices = getVerticesPtr(layout);

    for(int i = 0; i < getNumVertices(); ++i) {
      Vertex2D& vertex = vertices[i];

      for(int j = 0; j < layout->PositionNumElement; ++j)
        EXPECT_EQ(vertex.Position[j], j) << "vertex index " << i;

      for(int j = 0; j < layout->TexCoordNumElement; ++j)
        EXPECT_EQ(vertex.TexCoord[j], j) << "vertex index " << i;

      for(int j = 0; j < layout->ColorNumElement; ++j)
        EXPECT_EQ(vertex.Color[j], j) << "vertex index " << i;
    }
  }
};

TYPED_TEST(GLVertexDataTest, ReadAndWrite) {
  std::vector<unsigned int> indices = {1, 2};

  {
    std::shared_ptr<GLVertexData> gldata = makeVertexData<TypeParam>(2, 2, true, true);

    TestWriteVisitor writeVisitor;
    gldata->accept(Buffer::LO_Discard, writeVisitor);

    //    gldata->writeIndex(indices.data(), indices.size());
    //    gldata->dump();

    TestReadVisitor readVisitor;
    gldata->accept(Buffer::LO_ReadOnly, readVisitor);

    gldata->writeIndex(indices.data(), indices.size());
    std::vector<unsigned int> indicesRef(2, 0);
    gldata->readIndex(indicesRef.data(), indices.size());
    EXPECT_EQ(indices, indicesRef);
  }

  {
    std::shared_ptr<GLVertexData> gldata = makeVertexData<TypeParam>(2, 2, false, false);

    TestWriteVisitor writeVisitor;
    gldata->accept(Buffer::LO_Discard, writeVisitor);

    TestReadVisitor readVisitor;
    gldata->accept(Buffer::LO_ReadOnly, readVisitor);

    gldata->writeIndex(indices.data(), indices.size());
    std::vector<unsigned int> indicesRef(2, 0);
    gldata->readIndex(indicesRef.data(), indices.size());
    EXPECT_EQ(indices, indicesRef);
  }
}

} // anonymous namespace
