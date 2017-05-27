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

#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Memory.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include "sequoia/Unittest/Environment.h"
#include "sequoia/Unittest/GL/GLRenderTest.h"
#include <gtest/gtest.h>

using namespace sequoia;
using namespace sequoia::unittest;
using namespace sequoia::render;

namespace {

class GLVertexArrayObjectTest : public GLRenderTest {};

/// @brief Dummy VertexData
template <class VertexDataType>
struct TestVertexData {
  TestVertexData(std::size_t numVertices)
      : Data((VertexDataType*)memory::aligned_alloc(numVertices * sizeof(VertexDataType))),
        NumVertices(numVertices), Layout(VertexDataType::getLayout()) {
    std::memset(Data, 0, numVertices * sizeof(VertexDataType));
  }

  ~TestVertexData() { memory::aligned_free(Data); }

  VertexDataType* Data;
  std::size_t NumVertices;
  const VertexLayout* Layout;
};

TEST_F(GLVertexArrayObjectTest, Vertex3D) {
  RenderSystem& rsys = RenderSystem::getSingleton();

  std::unique_ptr<VertexArrayObject> vao = rsys.createVertexArrayObject(getWindow());
  GLVertexArrayObject* glvao = dyn_cast<GLVertexArrayObject>(vao.get());

  auto vertexData = std::make_unique<TestVertexData<Vertex3D>>(64);
  glvao->attachVertexData(vertexData->Data, vertexData->NumVertices, vertexData->Layout,
                          VertexArrayObject::BK_StaticWriteOnly);
  
  const VertexLayout* layout = vertexData->Layout;  
  glvao->bind();
  
  // Check Position attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_TRUE(enabled);

    GLint bufferID;
    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                        &bufferID);
    EXPECT_EQ(bufferID, glvao->getVAOID());

    GLenum type;
    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
    EXPECT_EQ(type, GL_FLOAT);

    GLint normalized;
    glGetVertexAttribiv(GLVertexAttribute::Position, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                        &normalized);
    EXPECT_FALSE(normalized);

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
  
  // Check Normal attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_TRUE(enabled);

    GLint bufferID;
    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                        &bufferID);
    EXPECT_EQ(bufferID, glvao->getVAOID());

    GLenum type;
    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
    EXPECT_EQ(type, GL_FLOAT);

    GLint normalized;
    glGetVertexAttribiv(GLVertexAttribute::Normal, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                        &normalized);
    EXPECT_FALSE(normalized);

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
  
  // Check TexCoord attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_TRUE(enabled);

    GLint bufferID;
    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                        &bufferID);
    EXPECT_EQ(bufferID, glvao->getVAOID());

    GLenum type;
    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
    EXPECT_EQ(type, GL_FLOAT);

    GLint normalized;
    glGetVertexAttribiv(GLVertexAttribute::TexCoord, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                        &normalized);
    EXPECT_FALSE(normalized);

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
  
  // Check Color attribute
  {
    GLint enabled;
    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
    EXPECT_TRUE(enabled);

    GLint bufferID;
    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING,
                        &bufferID);
    EXPECT_EQ(bufferID, glvao->getVAOID());

    GLenum type;
    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
    EXPECT_EQ(type, GL_FLOAT);

    GLint normalized;
    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
                        &normalized);
    EXPECT_FALSE(normalized);

    GLint size;
    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
    EXPECT_EQ(size, layout->ColorNumElement);

    GLint stride;
    glGetVertexAttribiv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
    EXPECT_EQ(stride, layout->SizeOf);

    void* pointer;
    glGetVertexAttribPointerv(GLVertexAttribute::Color, GL_VERTEX_ATTRIB_ARRAY_POINTER,
                              &pointer);
    EXPECT_EQ((std::size_t)pointer, layout->ColorOffset);
  }
  glvao->unbind();
  
  
  //glvao->updateDevice(0, glvao->getNumVertices());
}

} // anonymous namespace
