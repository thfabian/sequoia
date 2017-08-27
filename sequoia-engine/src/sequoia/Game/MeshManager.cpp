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

#include "sequoia/Core/HashCombine.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Game/Exception.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/MeshManager.h"
#include "sequoia/Render/RenderSystem.h"
#include <tiny_obj_loader.h>

namespace sequoia {

namespace game {

std::shared_ptr<Mesh> MeshManager::load(const std::string& name, const std::shared_ptr<File>& file,
                                        bool modifiable, const MeshParameter& param,
                                        render::Buffer::UsageHint usage) {
  switch(file->getType()) {
  case core::FileType::Obj:
    return loadObjMesh(name, file, modifiable, param, usage);
  default:
    SEQUOIA_THROW(GameException, "cannot load mesh %s: unknown format", file->getPath());
    return nullptr;
  }
}

//===------------------------------------------------------------------------------------------===//
//    Wavefornt OBJ
//===------------------------------------------------------------------------------------------===//

std::shared_ptr<Mesh> MeshManager::loadObjMesh(const std::string& name,
                                               const std::shared_ptr<File>& file, bool modifiable,
                                               const MeshParameter& param,
                                               render::Buffer::UsageHint usage) {
  LOG(DEBUG) << "Loading obj mesh \"" << name << "\" ...";

  LOG(DEBUG) << "Successfully loaded obj mesh \"" << name << "\"";
  return nullptr;
}

//===------------------------------------------------------------------------------------------===//
//    Cube
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
static float CubeVertexData[] = {   0.5, 0.5, 0.5,   0, 0, 1,   0, 0, 0,   1, 1,      // v0 (front)
                                   -0.5, 0.5, 0.5,   0, 0, 1,   0, 0, 0,   0, 1,      // v1
                                   -0.5,-0.5, 0.5,   0, 0, 1,   0, 0, 0,   0, 0,      // v2
                                    0.5,-0.5, 0.5,   0, 0, 1,   0, 0, 0,   1, 0,      // v3
            
                                    0.5, 0.5, 0.5,   1, 0, 0,   0, 0, 0,   0, 1,      // v0 (right)
                                    0.5,-0.5, 0.5,   1, 0, 0,   0, 0, 0,   0, 0,      // v3
                                    0.5,-0.5,-0.5,   1, 0, 0,   0, 0, 0,   1, 0,      // v4
                                    0.5, 0.5,-0.5,   1, 0, 0,   0, 0, 0,   1, 1,      // v5
            
                                    0.5, 0.5, 0.5,   0, 1, 0,   0, 0, 0,   1, 0,      // v0 (top)
                                    0.5, 0.5,-0.5,   0, 1, 0,   0, 0, 0,   1, 1,      // v5
                                   -0.5, 0.5,-0.5,   0, 1, 0,   0, 0, 0,   0, 1,      // v6
                                   -0.5, 0.5, 0.5,   0, 1, 0,   0, 0, 0,   0, 0,      // v1
            
                                   -0.5, 0.5, 0.5,  -1, 0, 0,   0, 0, 0,   1, 1,      // v1 (left)
                                   -0.5, 0.5,-0.5,  -1, 0, 0,   0, 0, 0,   0, 1,      // v6
                                   -0.5,-0.5,-0.5,  -1, 0, 0,   0, 0, 0,   0, 0,      // v7
                                   -0.5,-0.5, 0.5,  -1, 0, 0,   0, 0, 0,   1, 0,      // v2
            
                                   -0.5,-0.5,-0.5,   0,-1, 0,   0, 0, 0,   0, 0,      // v7 (bottom)
                                    0.5,-0.5,-0.5,   0,-1, 0,   0, 0, 0,   1, 0,      // v4
                                    0.5,-0.5, 0.5,   0,-1, 0,   0, 0, 0,   1, 1,      // v3
                                   -0.5,-0.5, 0.5,   0,-1, 0,   0, 0, 0,   0, 1,      // v2
            
                                    0.5,-0.5,-0.5,   0, 0,-1,   0, 0, 0,   0, 0,      // v4 (back)
                                   -0.5,-0.5,-0.5,   0, 0,-1,   0, 0, 0,   1, 0,      // v7
                                   -0.5, 0.5,-0.5,   0, 0,-1,   0, 0, 0,   1, 1,      // v6
                                    0.5, 0.5,-0.5,   0, 0,-1,   0, 0, 0,   0, 1  };   // v5

static unsigned int CubeIndices[]  = {   0, 1, 2,   2, 3, 0,                    // front
                                         4, 5, 6,   6, 7, 4,                    // right
                                         8, 9,10,  10,11, 8,                    // top
                                        12,13,14,  14,15,12,                    // left
                                        16,17,18,  18,19,16,                    // bottom
                                        20,21,22,  22,23,20 };                  // back

// clang-format on

} // anonymous namespace

std::shared_ptr<Mesh> MeshManager::createCube(const std::string& name, bool modifiable,
                                              const MeshParameter& param,
                                              render::Buffer::UsageHint usage) {
  LOG(DEBUG) << "Creating cube mesh \"" << name << "\" ...";

  std::shared_ptr<render::VertexData> vertexData = nullptr;
  VertexDataAccessRecord* record = nullptr;

  std::size_t hash = std::hash<MeshParameter>()(param);

  // Has the vertex-data already been created?
  cubeMutex_.lock();

  auto it = cubeMeshLookupMap_.find(hash);
  if(it != cubeMeshLookupMap_.end()) {
    record = it->second.get();
  } else {
    record = cubeMeshLookupMap_.emplace(hash, std::make_unique<VertexDataAccessRecord>())
                 .first->second.get();
  }
  record->Mutex.lock();

  cubeMutex_.unlock();

  if(record->Index != -1) {
    SEQUOIA_LOCK_GUARD(vertexDataMutex_);
    vertexData = vertexData_[record->Index];
  } else {
    std::size_t numVertices = 24;
    std::size_t numIndices = 36;

    render::VertexDataParameter vertexParam(render::VertexData::DM_Triangles,
                                            render::Vertex3D::getLayout(), numVertices, numIndices,
                                            usage);
    vertexParam.IndexType = render::IndexBuffer::IT_UInt32;
    vertexParam.UseVertexShadowBuffer = true;
    vertexParam.UseIndexShadowBuffer = false;

    vertexData = Game::getSingleton().getRenderSystem()->createVertexData(vertexParam);

    vertexData->writeVertex(render::Buffer::LO_Discard, [&](render::Vertex3D* vertices) {
      for(std::size_t i = 0; i < numVertices; ++i) {
        render::Vertex3D& vertex = vertices[i];

        // Position
        for(int j = 0; j < 3; ++j)
          vertex.Position[j] = CubeVertexData[i * CubeVertexDataStride + j];

        // Normal
        for(int j = 0; j < 3; ++j)
          vertex.Normal[j] = CubeVertexData[i * CubeVertexDataStride + 3 + j];

        // Color
        static_assert(std::is_integral<render::Vertex3D::ColorType>::value,
                      "color should be integral");

        constexpr auto maxRGBValue = std::numeric_limits<render::Vertex3D::ColorType>::max();
        for(int j = 0; j < 3; ++j)
          vertex.Color[j] = maxRGBValue * CubeVertexData[i * CubeVertexDataStride + 6 + j];
        vertex.Color[3] = maxRGBValue;

        // TexCoord
        vertex.TexCoord[0] = CubeVertexData[i * CubeVertexDataStride + 9];
        if(param.TexCoordInvertV)
          vertex.TexCoord[1] = 1.0f - CubeVertexData[i * CubeVertexDataStride + 10];
        else
          vertex.TexCoord[1] = CubeVertexData[i * CubeVertexDataStride + 10];
      }
    });

    // Set bounding box
    vertexData->setAxisAlignedBox(
        math::AxisAlignedBox(math::vec3(-0.5, -0.5, -0.5), math::vec3(0.5, 0.5, 0.5)));

    // Set indices
    vertexData->writeIndex(CubeIndices, numIndices);

    // Register the data
    SEQUOIA_LOCK_GUARD(vertexDataMutex_);
    vertexData_.emplace_back(vertexData);
    record->Index = vertexData_.size() - 1;
  }

  record->Mutex.unlock();

  LOG(DEBUG) << "Successfully created cube mesh \"" << name << "\"";
  return std::make_shared<Mesh>(name, vertexData, modifiable);
}

//===------------------------------------------------------------------------------------------===//
//    Grid
//===------------------------------------------------------------------------------------------===//

std::shared_ptr<Mesh> MeshManager::createGrid(const std::string& name, unsigned int N,
                                              bool modifiable, const MeshParameter& param,
                                              render::Buffer::UsageHint usage) {

  LOG(DEBUG) << "Creating grid mesh \"" << name << "\" ...";

  std::shared_ptr<render::VertexData> vertexData = nullptr;
  VertexDataAccessRecord* record = nullptr;

  std::size_t hash = 0;
  core::hashCombine(hash, N, std::hash<MeshParameter>()(param));

  // Has the vertex-data already been created?
  gridMutex_.lock();

  auto it = gridMeshLookupMap_.find(hash);
  if(it != gridMeshLookupMap_.end()) {
    record = it->second.get();
  } else {
    record = gridMeshLookupMap_.emplace(hash, std::make_unique<VertexDataAccessRecord>())
                 .first->second.get();
  }
  record->Mutex.lock();

  gridMutex_.unlock();

  if(record->Index != -1) {
    SEQUOIA_LOCK_GUARD(vertexDataMutex_);
    vertexData = vertexData_[record->Index];
  } else {
    unsigned int numVertices = N * N;
    unsigned int numIndices = 6 * (N - 1) * (N - 1);

    render::VertexDataParameter vertexParam(render::VertexData::DM_Triangles,
                                            render::Vertex3D::getLayout(), numVertices, numIndices,
                                            usage);

    vertexParam.IndexType = render::IndexBuffer::IT_UInt32;
    vertexParam.UseVertexShadowBuffer = true;
    vertexParam.UseIndexShadowBuffer = false;

    vertexData = Game::getSingleton().getRenderSystem()->createVertexData(vertexParam);

    const float dx = 1.0f / (N - 1);
    vertexData->writeVertex(render::Buffer::LO_Discard, [&](render::Vertex3D* vertices) {
      for(unsigned int i = 0; i < N; ++i) {
        for(unsigned int j = 0; j < N; ++j) {
          render::Vertex3D& vertex = vertices[i * N + j];

          // Position
          vertex.Position[0] = dx * (i - 0.5f * (N - 1));
          vertex.Position[1] = 0.0f;
          vertex.Position[2] = dx * (j - 0.5f * (N - 1));

          // Normal
          vertex.Normal[0] = 0;
          vertex.Normal[1] = 1;
          vertex.Normal[2] = 0;

          // Color
          static_assert(std::is_integral<render::Vertex3D::ColorType>::value,
                        "color should be integral");

          constexpr auto maxRGBValue = std::numeric_limits<render::Vertex3D::ColorType>::max();
          vertex.Color[0] = vertex.Color[1] = vertex.Color[2] = 0;
          vertex.Color[3] = maxRGBValue;

          // TexCoord
          float u = i;
          float v = j;

          vertex.TexCoord[0] = u;
          vertex.TexCoord[1] = param.TexCoordInvertV ? 1.0f - v : v;
        }
      }
    });

    //    0 ------ 3      1. Triangle { 1, 0, 3}
    //    |        |      2. Triangle { 4, 1, 3}
    //    |        |
    //    1 ------ 4
    //
    //   index = [ 1, 0, 3, 4, 1, 3 ]
    //
    vertexData->getIndexBuffer()->lock(render::Buffer::LO_Discard);
    unsigned int* indices = static_cast<unsigned int*>(vertexData->getIndexBuffer()->get());
    for(unsigned int i = 0; i < N - 1; ++i) {
      for(unsigned int j = 0; j < N - 1; ++j) {
        unsigned int tri = 6 * (i * (N - 1) + j);

        // We always store 2 triangles at once
        indices[tri + 0] = i * N + j + 1;
        indices[tri + 1] = i * N + j;
        indices[tri + 2] = i * N + j + N;

        indices[tri + 3] = i * N + j + N + 1;
        indices[tri + 4] = i * N + j + 1;
        indices[tri + 5] = i * N + j + N;
      }
    }
    vertexData->getIndexBuffer()->unlock();

    // Register the data
    SEQUOIA_LOCK_GUARD(vertexDataMutex_);
    vertexData_.emplace_back(vertexData);
    record->Index = vertexData_.size() - 1;
  }

  record->Mutex.unlock();

  // TODO: Copy for modifieable

  LOG(DEBUG) << "Successfully created grid mesh \"" << name << "\"";
  return std::make_shared<Mesh>(name, vertexData, modifiable);
}

std::size_t MeshManager::getNumMeshes() const { return vertexData_.size(); }

void MeshManager::freeUnusedMeshes() {
  SEQUOIA_LOCK_GUARD(vertexDataMutex_);

  auto it = std::remove_if(
      vertexData_.begin(), vertexData_.end(),
      [](const std::shared_ptr<render::VertexData>& meshPtr) { return meshPtr.use_count() == 1; });

  if(it != vertexData_.end()) {
    LOG(INFO) << "Removing " << std::distance(it, vertexData_.end()) << " unused meshes";
    vertexData_.erase(it, vertexData_.end());
  }
}

} // namespace game

} // namespace sequoia
