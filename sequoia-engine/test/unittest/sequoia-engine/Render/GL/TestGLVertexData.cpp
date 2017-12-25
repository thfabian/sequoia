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

#include "sequoia-engine/Core/Casting.h"
#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/GL/GLVertexAttribute.h"
#include "sequoia-engine/Render/GL/GLVertexData.h"
#include "sequoia-engine/Render/VertexAdapter.h"
#include "sequoia-engine/Unittest/RenderSetup.h"
#include "sequoia-engine/Unittest/TestEnvironment.h"
#include <boost/preprocessor/seq/enum.hpp>
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

SEQUOIA_TESTCASEFIXTURE_TEMPLATE(GLVertexDataTest, RenderSetup);

using VertexTypes = testing::Types<BOOST_PP_SEQ_ENUM(SEQUOIA_VERTICES)>;
TYPED_TEST_CASE(GLVertexDataTest, VertexTypes);

/// @brief Create a GLVertexData and allocate space for `numVertices` and `numIndices`
template <class VertexDataType>
std::shared_ptr<GLVertexData> makeVertexData(std::size_t numVertices, std::size_t numIndices,
                                             bool useVertexShadowBuffer = true,
                                             bool useIndexShadowBuffer = true) {
  RenderSystem& rsys = RenderSystem::getSingleton();
  VertexDataParameter param(render::VertexData::DM_Triangles, VertexDataType::getLayout(),
                            numVertices, numIndices, Buffer::UH_StaticWriteOnly);
  param.UseVertexShadowBuffer = useVertexShadowBuffer;
  param.UseIndexShadowBuffer = useIndexShadowBuffer;
  auto vertexData = rsys.createVertexData(param);
  EXPECT_TRUE(core::isa<GLVertexData>(vertexData.get()));
  return core::dyn_pointer_cast<GLVertexData>(vertexData);
}

TYPED_TEST(GLVertexDataTest, Allocation) {
  std::shared_ptr<GLVertexData> gldata1 = makeVertexData<TypeParam>(32, 16);
  EXPECT_EQ(gldata1->getNumVertices(), 32);
  EXPECT_EQ(gldata1->getNumIndices(), 16);
  EXPECT_TRUE(gldata1->hasIndices());

  std::shared_ptr<GLVertexData> gldata2 = makeVertexData<TypeParam>(16, 0);
  EXPECT_EQ(gldata2->getNumVertices(), 16);
  EXPECT_EQ(gldata2->getNumIndices(), 0);
  EXPECT_FALSE(gldata2->hasIndices());
}

TYPED_TEST(GLVertexDataTest, VertexAttributes) {
  std::shared_ptr<GLVertexData> gldata = makeVertexData<TypeParam>(8, 8);
  const VertexLayout& layout = gldata->getVertexBuffer()->getLayout();

  gldata->bind();

  // Check Position attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_EQ(enabled, (GLint)layout.hasPosition());

    if(layout.hasPosition()) {
      GLint bufferID;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &bufferID);
      EXPECT_EQ(bufferID, gldata->getVAOID());

      GLenum type;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
      EXPECT_EQ(type, GLVertexData::getGLType(layout.Position.Type));

      GLint normalized;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                          &normalized);
      EXPECT_EQ(normalized, (GLint)layout.Position.Normalize);

      GLint size;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
      EXPECT_EQ(size, layout.Position.NumElements);

      GLint stride;
      glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
      EXPECT_EQ(stride, layout.SizeOf);

      void* pointer;
      glGetVertexAttribPointerv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_POINTER,
                                &pointer);
      EXPECT_EQ((std::size_t)pointer, layout.Position.Offset);
    }
  }

  // Check Normal attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_EQ(enabled, (GLint)layout.hasNormal());

    if(layout.hasNormal()) {
      GLint bufferID;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &bufferID);
      EXPECT_EQ(bufferID, gldata->getVAOID());

      GLenum type;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
      EXPECT_EQ(type, GLVertexData::getGLType(layout.Normal.Type));

      GLint normalized;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                          &normalized);
      EXPECT_EQ(normalized, (GLint)layout.Normal.Normalize);

      GLint size;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
      EXPECT_EQ(size, layout.Normal.NumElements);

      GLint stride;
      glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
      EXPECT_EQ(stride, layout.SizeOf);

      void* pointer;
      glGetVertexAttribPointerv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_POINTER,
                                &pointer);
      EXPECT_EQ((std::size_t)pointer, layout.Normal.Offset);
    }
  }

  // Check TexCoord attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_EQ(enabled, (GLint)layout.hasTexCoord());

    if(layout.hasTexCoord()) {
      GLint bufferID;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &bufferID);
      EXPECT_EQ(bufferID, gldata->getVAOID());

      GLenum type;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
      EXPECT_EQ(type, GLVertexData::getGLType(layout.TexCoord.Type));

      GLint normalized;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                          &normalized);
      EXPECT_EQ(normalized, (GLint)layout.TexCoord.Normalize);

      GLint size;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
      EXPECT_EQ(size, layout.TexCoord.NumElements);

      GLint stride;
      glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
      EXPECT_EQ(stride, layout.SizeOf);

      void* pointer;
      glGetVertexAttribPointerv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_POINTER,
                                &pointer);
      EXPECT_EQ((std::size_t)pointer, layout.TexCoord.Offset);
    }
  }

  // Check Color attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_EQ(enabled, (GLint)layout.hasColor());

    if(layout.hasColor()) {
      GLint bufferID;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                          &bufferID);
      EXPECT_EQ(bufferID, gldata->getVAOID());

      GLenum type;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
      EXPECT_EQ(type, GLVertexData::getGLType(layout.Color.Type));

      GLint normalized;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized);
      EXPECT_EQ(normalized, (GLint)layout.Color.Normalize);

      GLint size;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
      EXPECT_EQ(size, layout.Color.NumElements);

      GLint stride;
      glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
      EXPECT_EQ(stride, layout.SizeOf);

      void* pointer;
      glGetVertexAttribPointerv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);
      EXPECT_EQ((std::size_t)pointer, layout.Color.Offset);
    }
  }
  
  gldata->unbind();
}

/// @brief Write to all vertex attributes the index of the vertex
static void writeVertex(VertexBuffer* buffer) {
  BufferGuard guard(buffer, Buffer::LO_Discard);

  const VertexLayout& layout = buffer->getLayout();
  const std::size_t numVertices = buffer->getNumVertices();

  VertexAdapter adapter(layout);
  Byte* vertexPtr = guard.getAsByte();

  for(std::size_t i = 0; i < numVertices; ++i, vertexPtr += layout.SizeOf) {
    adapter.clear();

    adapter.setPosition(math::vec4(i));
    adapter.setNormal(math::vec4(i));
    adapter.setTexCoord(math::vec4(i));
    adapter.setColor(Color(i, i, i, i));

    adapter.copyTo(vertexPtr);
  }
}

/// @brief Real all vertex attributes and check if each attribute contains the vertex index
static void readVertex(VertexBuffer* buffer) {
  BufferGuard guard(buffer, Buffer::LO_ReadOnly);

  const VertexLayout& layout = buffer->getLayout();
  const std::size_t numVertices = buffer->getNumVertices();

  VertexAdapter adapter(layout);
  Byte* vertexPtr = guard.getAsByte();

  for(std::size_t i = 0; i < numVertices; ++i, vertexPtr += layout.SizeOf) {
    adapter.copyFrom(vertexPtr);

    // Position
    auto position = adapter.getPosition();
    for(int j = 0; j < layout.Position.NumElements; ++j)
      EXPECT_FLOAT_EQ(position[j], i) << "Vertex index: " << i << ", Attribute: Position[" << j
                                      << "]";

    // Normal
    auto normal = adapter.getNormal();
    for(int j = 0; j < layout.Normal.NumElements; ++j)
      EXPECT_FLOAT_EQ(normal[j], i) << "Vertex index: " << i << ", Attribute: Normal[" << j << "]";

    // TexCoord
    auto texCoord = adapter.getTexCoord();
    for(int j = 0; j < layout.TexCoord.NumElements; ++j)
      EXPECT_FLOAT_EQ(texCoord[j], i) << "Vertex index: " << i << ", Attribute: TexCoord[" << j
                                      << "]";

    // Color
    auto color = adapter.getColor();
    for(int j = 0; j < Color::NumChannels; ++j)
      EXPECT_EQ(color[j], i) << "Vertex index: " << i << ", Attribute: Color[" << j << "]";
  }
}

TYPED_TEST(GLVertexDataTest, ReadAndWrite) {
  using IndexType = unsigned int;
  std::vector<IndexType> indices = {1, 2};

  {
    std::shared_ptr<GLVertexData> gldata = makeVertexData<TypeParam>(2, 2, true, true);

    // Vertices
    writeVertex(gldata->getVertexBuffer());
    readVertex(gldata->getVertexBuffer());

    // Indices
    gldata->getIndexBuffer()->write(indices.data(), 0, indices.size() * sizeof(IndexType));
    std::vector<unsigned int> indicesRef(2, 0);
    gldata->getIndexBuffer()->read(0, indices.size() * sizeof(IndexType), indicesRef.data());
    EXPECT_EQ(indices, indicesRef);
  }

  {
    std::shared_ptr<GLVertexData> gldata = makeVertexData<TypeParam>(2, 2, false, false);

    // Vertices
    writeVertex(gldata->getVertexBuffer());
    readVertex(gldata->getVertexBuffer());

    // Indices
    gldata->getIndexBuffer()->write(indices.data(), 0, indices.size() * sizeof(IndexType));
    std::vector<unsigned int> indicesRef(2, 0);
    gldata->getIndexBuffer()->read(0, indices.size() * sizeof(IndexType), indicesRef.data());
    EXPECT_EQ(indices, indicesRef);
  }
}

} // anonymous namespace
