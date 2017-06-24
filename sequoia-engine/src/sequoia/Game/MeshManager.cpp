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

#include "sequoia/Game/MeshManager.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Render/RenderSystem.h"

namespace sequoia {

namespace game {

MeshManager::MeshManager(render::RenderTarget* target) : target_(target) {
  staticCubeMeshDataIdx_ = -1;
}

std::shared_ptr<Mesh> MeshManager::load(const std::string& name, const std::shared_ptr<File>& file,
                                        bool modifiable, BufferUsageKind usage) {
  return nullptr;
}

//===------------------------------------------------------------------------------------------===//
//    Cube Mesh
//===------------------------------------------------------------------------------------------===//

namespace {

// clang-format off

// Simple cube vertex data in the format {Position, Normal, Color, UV}
//
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
//
static int CubeVertexDataStride = 11;
static float CubeVertexData[] = {   1, 1, 1,   0, 0, 1,   1, 1, 1,   1, 1,      // v0 (front)
                                   -1, 1, 1,   0, 0, 1,   1, 1, 0,   0, 1,      // v1
                                   -1,-1, 1,   0, 0, 1,   1, 0, 0,   0, 0,      // v2
                                    1,-1, 1,   0, 0, 1,   1, 0, 1,   1, 0,      // v3
            
                                    1, 1, 1,   1, 0, 0,   1, 1, 1,   0, 1,      // v0 (right)
                                    1,-1, 1,   1, 0, 0,   1, 0, 1,   0, 0,      // v3
                                    1,-1,-1,   1, 0, 0,   0, 0, 1,   1, 0,      // v4
                                    1, 1,-1,   1, 0, 0,   0, 1, 1,   1, 1,      // v5
            
                                    1, 1, 1,   0, 1, 0,   1, 1, 1,   1, 0,      // v0 (top)
                                    1, 1,-1,   0, 1, 0,   0, 1, 1,   1, 1,      // v5
                                   -1, 1,-1,   0, 1, 0,   0, 1, 0,   0, 1,      // v6
                                   -1, 1, 1,   0, 1, 0,   1, 1, 0,   0, 0,      // v1
            
                                   -1, 1, 1,  -1, 0, 0,   1, 1, 0,   1, 1,      // v1 (left)
                                   -1, 1,-1,  -1, 0, 0,   0, 1, 0,   0, 1,      // v6
                                   -1,-1,-1,  -1, 0, 0,   0, 0, 0,   0, 0,      // v7
                                   -1,-1, 1,  -1, 0, 0,   1, 0, 0,   1, 0,      // v2
            
                                   -1,-1,-1,   0,-1, 0,   0, 0, 0,   0, 0,      // v7 (bottom)
                                    1,-1,-1,   0,-1, 0,   0, 0, 1,   1, 0,      // v4
                                    1,-1, 1,   0,-1, 0,   1, 0, 1,   1, 1,      // v3
                                   -1,-1, 1,   0,-1, 0,   1, 0, 0,   0, 1,      // v2
            
                                    1,-1,-1,   0, 0,-1,   0, 0, 1,   0, 1,      // v4 (back)
                                   -1,-1,-1,   0, 0,-1,   0, 0, 0,   1, 1,      // v7
                                   -1, 1,-1,   0, 0,-1,   0, 1, 0,   1, 0,      // v6
                                    1, 1,-1,   0, 0,-1,   0, 1, 1,   0, 0  };   // v5

static unsigned int CubeIndices[]  = {   0, 1, 2,   2, 3, 0,                    // front
                                         4, 5, 6,   6, 7, 4,                    // right
                                         8, 9,10,  10,11, 8,                    // top
                                        12,13,14,  14,15,12,                    // left
                                        16,17,18,  18,19,16,                    // bottom
                                        20,21,22,  22,23,20 };                  // back

// clang-format on

} // anonymous namespace

std::shared_ptr<Mesh> MeshManager::createCube(const std::string& name, bool modifiable,
                                              BufferUsageKind usage) {
  LOG(INFO) << "Creating cube mesh \"" << name << "\" ...";

  if(staticCubeMeshDataIdx_ == -1) {
    std::size_t numVertices = 24;
    std::size_t numIndices = 36;

    std::shared_ptr<render::VertexData> data(
        new render::VertexData(render::Vertex3D::getLayout(), render::VertexData::DM_Triangles,
                               numVertices, numIndices, false));

    // Set vertex data
    render::Vertex3D* vertex = (render::Vertex3D*)data->getVerticesPtr();
    for(std::size_t i = 0; i < numVertices; ++i) {
      // Position
      for(int j = 0; j < 3; ++j)
        vertex[i].Position[j] = CubeVertexData[i * CubeVertexDataStride + j];

      // Normal
      for(int j = 0; j < 3; ++j)
        vertex[i].Normal[j] = CubeVertexData[i * CubeVertexDataStride + 3 + j];

      // Color
      static_assert(std::is_integral<render::Vertex3D::ColorType>::value,
                    "color should be integral");

      constexpr auto maxRGBValue = std::numeric_limits<render::Vertex3D::ColorType>::max();
      for(int j = 0; j < 3; ++j)
        vertex[i].Color[j] = maxRGBValue * CubeVertexData[i * CubeVertexDataStride + 6 + j];
      vertex[i].Color[3] = maxRGBValue;

      // TexCoord
      for(int j = 0; j < 2; ++j)
        vertex[i].TexCoord[j] = CubeVertexData[i * CubeVertexDataStride + 9 + j];
    }

    // Set bounding box
    data->setAxisAlignedBox(math::AxisAlignedBox(math::vec3(-1, -1, -1), math::vec3(1, 1, 1)));

    // Set indices
    std::memcpy(data->getIndicesPtr(), CubeIndices, numIndices * sizeof(render::VertexIndexType));

    // data->dump();

    // Set the VAO
    data->setVertexArrayObject(
        render::RenderSystem::getSingleton().createVertexArrayObject(target_), usage);
    data->getVertexArrayObject()->writeVertexData(0, numVertices);
    data->getVertexArrayObject()->writeIndexData(0, numIndices);

    vertexDataList_.emplace_back(data);
    staticCubeMeshDataIdx_ = vertexDataList_.size() - 1;
  }

  const std::shared_ptr<render::VertexData>& data = vertexDataList_[staticCubeMeshDataIdx_];

  LOG(INFO) << "Successfully created cube mesh \"" << name << "\"";
  return std::make_shared<Mesh>(name, data, modifiable);
}

std::size_t MeshManager::getNumMeshes() const { return vertexDataList_.size(); }

void MeshManager::freeUnusedMeshes() {
  auto it = std::remove_if(
      vertexDataList_.begin(), vertexDataList_.end(),
      [](const std::shared_ptr<render::VertexData>& meshPtr) { return meshPtr.use_count() == 1; });

  if(it != vertexDataList_.end()) {
    LOG(INFO) << "Removing " << std::distance(it, vertexDataList_.end()) << " unused meshes";
    vertexDataList_.erase(it, vertexDataList_.end());
  }
}

} // namespace game

} // namespace sequoia
