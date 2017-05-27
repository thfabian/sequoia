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

#include "sequoia/Core/Memory.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Render/RenderSystem.h"

namespace sequoia {

namespace game {

static void allocate(Mesh::Data* data, std::size_t numVertices,
                     const render::VertexLayout* layout) {
  data->DataPtr = memory::aligned_alloc(numVertices * layout->SizeOf);
  data->NumVertices = numVertices;
}

static void free(Mesh::Data* data) noexcept {
  memory::aligned_free(data->DataPtr);
  data->NumVertices = 0;
  data->Layout = nullptr;
  data->AAB.clear();
}

MeshManager::MeshManager() : staticCubeMeshDataIdx_(-1) {}

std::shared_ptr<Mesh> MeshManager::load(render::RenderTarget* target, const std::string& name,
                                        const platform::String& path, bool copy,
                                        BufferUsageKind usage) {
  return nullptr;
}

//===------------------------------------------------------------------------------------------===//
//    Cube Mesh
//===------------------------------------------------------------------------------------------===//

namespace {

// clang-format off

// Simple cube vertex data in the format {Position, Normal, Color}
//
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
//
static float CubeVertexData[] = {   1, 1, 1,   0, 0, 1,   1, 1, 1,              // v0 (front)
                                   -1, 1, 1,   0, 0, 1,   1, 1, 0,              // v1
                                   -1,-1, 1,   0, 0, 1,   1, 0, 0,              // v2
                                    1,-1, 1,   0, 0, 1,   1, 0, 1,              // v3
            
                                    1, 1, 1,   1, 0, 0,   1, 1, 1,              // v0 (right)
                                    1,-1, 1,   1, 0, 0,   1, 0, 1,              // v3
                                    1,-1,-1,   1, 0, 0,   0, 0, 1,              // v4
                                    1, 1,-1,   1, 0, 0,   0, 1, 1,              // v5
            
                                    1, 1, 1,   0, 1, 0,   1, 1, 1,              // v0 (top)
                                    1, 1,-1,   0, 1, 0,   0, 1, 1,              // v5
                                   -1, 1,-1,   0, 1, 0,   0, 1, 0,              // v6
                                   -1, 1, 1,   0, 1, 0,   1, 1, 0,              // v1
            
                                   -1, 1, 1,  -1, 0, 0,   1, 1, 0,              // v1 (left)
                                   -1, 1,-1,  -1, 0, 0,   0, 1, 0,              // v6
                                   -1,-1,-1,  -1, 0, 0,   0, 0, 0,              // v7
                                   -1,-1, 1,  -1, 0, 0,   1, 0, 0,              // v2
            
                                   -1,-1,-1,   0,-1, 0,   0, 0, 0,              // v7 (bottom)
                                    1,-1,-1,   0,-1, 0,   0, 0, 1,              // v4
                                    1,-1, 1,   0,-1, 0,   1, 0, 1,              // v3
                                   -1,-1, 1,   0,-1, 0,   1, 0, 0,              // v2
            
                                    1,-1,-1,   0, 0,-1,   0, 0, 1,              // v4 (back)
                                   -1,-1,-1,   0, 0,-1,   0, 0, 0,              // v7
                                   -1, 1,-1,   0, 0,-1,   0, 1, 0,              // v6
                                    1, 1,-1,   0, 0,-1,   0, 1, 1 };            // v5

static unsigned int CubeIndices[]  = {   0, 1, 2,   2, 3, 0,                    // front
                                         4, 5, 6,   6, 7, 4,                    // right
                                         8, 9,10,  10,11, 8,                    // top
                                        12,13,14,  14,15,12,                    // left
                                        16,17,18,  18,19,16,                    // bottom
                                        20,21,22,  22,23,20 };                  // back
// clang-format on

} // anonymous namespace

std::shared_ptr<Mesh> MeshManager::createCube(render::RenderTarget* target, const std::string& name,
                                              bool copy) {

  if(staticCubeMeshDataIdx_ == -1) {
    std::shared_ptr<Mesh::Data> data = std::make_shared<Mesh::Data>();
    data->Layout = render::Vertex3D::getLayout();
    allocate(data.get(), 24, data->Layout);

    render::Vertex3D* vertex = (render::Vertex3D*)data->DataPtr;
    for(int i = 0; i < 24; ++i) {
      // Position
      std::memcpy(vertex[i].Position, &CubeVertexData[i * 9], 3 * sizeof(float));

      // Normal
      std::memcpy(vertex[i].Normal, &CubeVertexData[i * 9 + 3], 3 * sizeof(float));

      // TexCoord
      vertex[i].TexCoord[0] = vertex[i].TexCoord[1] = 0;

      // Color
      std::memcpy(vertex[i].Color, &CubeVertexData[i * 9 + 6], 3 * sizeof(float));
    }

    // Set bounding box
    data->AAB.setExtents(math::vec3(-1, -1, -1), math::vec3(1, 1, 1));

    // Set the VAO
    data->VAO = render::RenderSystem::getSingleton().createVertexArrayObject(target);
    data->VAO->attachVertexData(data->DataPtr, data->NumVertices, data->Layout,
                                BufferUsageKind::BK_StaticWriteOnly);

    // TODO: add the index data
    
    meshDataList_.emplace_back(<data);
    staticCubeMeshDataIdx_ = meshDataList_.size() - 1;
  }

  const std::shared_ptr<Mesh::Data>& data = meshDataList_[staticCubeMeshDataIdx_];

  std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(name);
  mesh->data_ = data;
  return mesh;
}

} // namespace game

} // namespace sequoia
