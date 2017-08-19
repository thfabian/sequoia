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

#include "sequoia/Render/VertexData.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

//TEST(VertexDataTest, WriteAndRead) {
//  VertexData data(Vertex3D::getLayout(), VertexData::DM_Triangles, 4, 0, true);

//  std::size_t numVertices = data.getNumVertices();

//  data.write([&numVertices](Vertex3D* vertices) {
//    for(int i = 0; i < numVertices; ++i) {
//      vertices[i].Position[0] = i;
//      vertices[i].Normal[0] = i;
//      vertices[i].TexCoord[0] = i;
//      vertices[i].Color[0] = i;
//    }
//  });

//  data.read([&numVertices](const Vertex3D* vertices) {
//    for(int i = 0; i < numVertices; ++i) {
//      EXPECT_EQ(vertices[i].Position[0], i) << "vertex index " << i;
//      EXPECT_EQ(vertices[i].Normal[0], i) << "vertex index " << i;
//      EXPECT_EQ(vertices[i].TexCoord[0], i) << "vertex index " << i;
//      EXPECT_EQ(vertices[i].Color[0], i) << "vertex index " << i;
//    }
//  });
//}

//class GLVertexArrayObjectTest : public TestFixture<GLRenderSetup> {};

//template <class VertexDataType>
//std::unique_ptr<VertexData> makeVertexData(std::size_t numVertices, std::size_t numIndices) {
//  return std::make_unique<VertexData>(VertexDataType::getLayout(), VertexData::DM_Triangles,
//                                      numVertices, numIndices, true);
//}

//TEST_F(GLVertexArrayObjectTest, Vertex3D) {
//  RenderSystem& rsys = RenderSystem::getSingleton();

//  auto vertexData = makeVertexData<Vertex3D>(64, 32);
//  vertexData->setVertexArrayObject(rsys.createVertexArrayObject(),
//                                   VertexArrayObject::BK_StaticWriteOnly);

//  const VertexLayout* layout = vertexData->getLayout();
//  GLVertexArrayObject* glvao = dyn_cast<GLVertexArrayObject>(vertexData->getVertexArrayObject());

//  EXPECT_TRUE(vertexData->hasVertexArrayObject());

//  glvao->bind();

//  // Check Position attribute
//  {
//    GLint enabled;
//    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
//    EXPECT_TRUE(enabled);

//    GLint bufferID;
//    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
//                        &bufferID);
//    EXPECT_EQ(bufferID, glvao->getVAOID());

//    GLenum type;
//    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
//    EXPECT_EQ(type, GL_FLOAT);

//    GLint normalized;
//    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
//                        &normalized);
//    EXPECT_FALSE(normalized);

//    GLint size;
//    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
//    EXPECT_EQ(size, layout->PositionNumElement);

//    GLint stride;
//    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
//    EXPECT_EQ(stride, Vertex3D::getLayout()->SizeOf);

//    void* pointer;
//    glGetVertexAttribPointerv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_POINTER,
//                              &pointer);
//    EXPECT_EQ((std::size_t)pointer, layout->PositionOffset);
//  }

//  // Check Normal attribute
//  {
//    GLint enabled;
//    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
//    EXPECT_TRUE(enabled);

//    GLint bufferID;
//    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
//                        &bufferID);
//    EXPECT_EQ(bufferID, glvao->getVAOID());

//    GLenum type;
//    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
//    EXPECT_EQ(type, GL_FLOAT);

//    GLint normalized;
//    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized);
//    EXPECT_FALSE(normalized);

//    GLint size;
//    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
//    EXPECT_EQ(size, layout->NormalNumElement);

//    GLint stride;
//    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
//    EXPECT_EQ(stride, Vertex3D::getLayout()->SizeOf);

//    void* pointer;
//    glGetVertexAttribPointerv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);
//    EXPECT_EQ((std::size_t)pointer, layout->NormalOffset);
//  }

//  // Check TexCoord attribute
//  {
//    GLint enabled;
//    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
//    EXPECT_TRUE(enabled);

//    GLint bufferID;
//    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
//                        &bufferID);
//    EXPECT_EQ(bufferID, glvao->getVAOID());

//    GLenum type;
//    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
//    EXPECT_EQ(type, GL_FLOAT);

//    GLint normalized;
//    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
//                        &normalized);
//    EXPECT_FALSE(normalized);

//    GLint size;
//    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
//    EXPECT_EQ(size, layout->TexCoordNumElement);

//    GLint stride;
//    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
//    EXPECT_EQ(stride, Vertex3D::getLayout()->SizeOf);

//    void* pointer;
//    glGetVertexAttribPointerv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_POINTER,
//                              &pointer);
//    EXPECT_EQ((std::size_t)pointer, layout->TexCoordOffset);
//  }

//  // Check Color attribute
//  {
//    GLint enabled;
//    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
//    EXPECT_TRUE(enabled);

//    GLint bufferID;
//    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bufferID);
//    EXPECT_EQ(bufferID, glvao->getVAOID());

//    GLenum type;
//    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
//    EXPECT_EQ(type, GL_UNSIGNED_BYTE);

//    GLint normalized;
//    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized);
//    EXPECT_TRUE(normalized);

//    GLint size;
//    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
//    EXPECT_EQ(size, layout->ColorNumElement);

//    GLint stride;
//    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
//    EXPECT_EQ(stride, Vertex3D::getLayout()->SizeOf);

//    void* pointer;
//    glGetVertexAttribPointerv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);
//    EXPECT_EQ((std::size_t)pointer, layout->ColorOffset);
//  }
//}


} // anonymous namespace
