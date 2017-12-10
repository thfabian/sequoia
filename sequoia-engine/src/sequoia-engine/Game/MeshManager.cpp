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

#include "sequoia-engine/Core/AlignedADT.h"
#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Game/Exception.h"
#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Game/MeshManager.h"
#include "sequoia-engine/Render/RenderSystem.h"
#include "sequoia-engine/Render/VertexAdapter.h"
#include "sequoia-engine/Core/STLExtras.h"
#include <algorithm>
#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>

namespace sequoia {

namespace game {

namespace {

class AssimpLogStream : public Assimp::LogStream {
  Assimp::Logger::ErrorSeverity sevritiy_;

public:
  AssimpLogStream(Assimp::Logger::ErrorSeverity sevritiy) : sevritiy_(sevritiy) {}

  void write(const char* message) {
    std::string msg(message);
    msg.pop_back();
    switch(sevritiy_) {
    case Assimp::Logger::Info:
      Log::info("Assimp: {}", msg);
      break;
    case Assimp::Logger::Warn:
      Log::warn("Assimp: {}", msg);
      break;
    case Assimp::Logger::Err:
      Log::error("Assimp: {}", msg);
      break;
    case Assimp::Logger::Debugging:
      Log::debug("Assimp: {}", msg);
      break;
    default:
      sequoia_unreachable("invalid sevritiy");
    }
  }
};

} // anonymous namespace

/// TODO:
///   - Rename the whole class to "ShapeManager"
///   - A shape contains "Mesh" and "Material" vectors.
///   - Implement Vritual FS to allow loading files from memory
/// 

MeshManager::MeshManager() {
  // Create Assimp logger
  Assimp::DefaultLogger::create("", core::Logger::getSingleton().getLevel() <= core::Logger::Debug
                                        ? Assimp::Logger::VERBOSE
                                        : Assimp::Logger::NORMAL);

  // Attach a stream for each sevritiy
  Assimp::DefaultLogger::get()->attachStream(new AssimpLogStream(Assimp::Logger::Info),
                                             Assimp::Logger::Info);
  Assimp::DefaultLogger::get()->attachStream(new AssimpLogStream(Assimp::Logger::Warn),
                                             Assimp::Logger::Warn);
  Assimp::DefaultLogger::get()->attachStream(new AssimpLogStream(Assimp::Logger::Err),
                                             Assimp::Logger::Err);
  Assimp::DefaultLogger::get()->attachStream(new AssimpLogStream(Assimp::Logger::Debugging),
                                             Assimp::Logger::Debugging);
  importer_ = std::make_shared<Assimp::Importer>();
}

MeshManager::~MeshManager() {
  importer_.reset();
  Assimp::DefaultLogger::kill();
}

std::shared_ptr<Mesh> MeshManager::load(const std::string& name, const std::shared_ptr<File>& file,
                                        bool modifiable, const MeshParameter& param,
                                        render::Buffer::UsageHint usage) {
  Log::debug("Loading mesh \"{}\" from \"{}\" ...", name, file->getPath());

  std::shared_ptr<render::VertexData> vertexData = nullptr;
  VertexDataAccessRecord* record = nullptr;

  internal::MeshInfo info{file, param};
  meshMutex_.lock();

  auto it = meshLookupMap_.find(info);
  if(it != meshLookupMap_.end())
    record = it->second.get();
  else
    record = meshLookupMap_.emplace(std::move(info), std::make_unique<VertexDataAccessRecord>())
                 .first->second.get();

  record->Mutex.lock();
  meshMutex_.unlock();

  if(record->Index != -1) {
    vertexDataMutex_.lock_read();
    vertexData = vertexData_[record->Index];
    vertexDataMutex_.unlock();
  } else {
#if 0
    
    // Default flags
    int flags = aiProcessPreset_TargetRealtime_Quality;
    //int flags = aiProcessPreset_TargetRealtime_Fast;    
  
    // Optional flags
    flags = param.TexCoordInvertV ? flags | aiProcess_FlipUVs : flags;
    
    // Load mesh
    const aiScene* scene = importer_->ReadFile(
        "/home/thfabian/Desktop/sequoia/sequoia-engine/test/ressource/sequoia-engine/"
        "Game/TestMeshManager/Cube.obj",
        flags);

    if(!scene)
      SEQUOIA_THROW(GameException, "failed to load mesh {}: {}", file->getPath(),
                    importer_->GetErrorString());
    
    std::vector<std::shared_ptr<Mesh>> meshes(scene->mNumMeshes);
//    std::vector<std::shared_ptr<Material>> materials(pScene->mNumMaterials);    
    
    core::aligned_vector<unsigned int> indexBuffer;
    core::aligned_vector<render::Vertex3D> vertexBuffer;
    
    for(int meshIdx = 0 ; meshIdx < meshes.size(); meshIdx++) {
      indexBuffer.clear();
      vertexBuffer.clear();

      const aiMesh* mesh = scene->mMeshes[meshIdx];
      
      const int numVertices = mesh->mNumVertices;
      const int mumFaces = mesh->mNumFaces;

      vertexBuffer.reserve(numVertices);
      indexBuffer.reserve(3 * mumFaces);
      
      // Vertices
      for(int vertexIdx = 0; vertexIdx < numVertices; ++vertexIdx) {
        render::Vertex3D vertex;
        
        // Position
        const aiVector3D& position = mesh->mVertices[vertexIdx];
        for(int j = 0; j < 3; ++j)
          vertex.Position[j] = position[j];

        // Normal
        const aiVector3D& normal = mesh->mNormals[vertexIdx];
        for(int j = 0; j < 3; ++j)
          vertex.Normal[j] = normal[j];
        
        // UV
        if(mesh->HasTextureCoords(0)) {
          const aiVector3D& texCoord = mesh->mTextureCoords[0][vertexIdx];
          for(int j = 0; j < 2; ++j)
            vertex.TexCoord[j] = texCoord[j];
        } else {
          for(int j = 0; j < 2; ++j)
            vertex.TexCoord[0] = 0;
        }
        
        // Color
        constexpr auto maxRGBValue = std::numeric_limits<render::Vertex3D::ColorType>::max();
        if(mesh->HasVertexColors(0)) {
          const aiColor4D& color = mesh->mColors[0][vertexIdx];
          for(int j = 0; j < 4; ++j)
            vertex.Color[j] = maxRGBValue * color[j];
        } else {
          for(int j = 0; j < 3; ++j)
            vertex.Color[j] = 0;
          vertex.Color[3] = maxRGBValue;
        }
        vertexBuffer.emplace_back(std::move(vertex));
      }
      
      // Indices
      for(int faceIDx = 0; faceIDx < mumFaces; ++faceIDx) {
        const aiFace& face = mesh->mFaces[faceIDx];
        SEQUOIA_ASSERT_MSG(face.mNumIndices == 3, "more than 3 vertices per face");
      
        indexBuffer.push_back(face.mIndices[0]);
        indexBuffer.push_back(face.mIndices[1]);
        indexBuffer.push_back(face.mIndices[2]);
      }
      
//      for(auto i : indexBuffer)
//        std::cout << i << std::endl;
      
//      for(auto v : vertexBuffer)
//        std::cout << render::Vertex3D::toString(v) << std::endl;
    }
      
    //    tinyobj::attrib_t attrib;
    //    std::vector<tinyobj::shape_t> shapes;
    //    std::vector<tinyobj::material_t> materials;

    //    std::string err;

    //    std::unique_ptr<std::istringstream> objStream =
    //        std::make_unique<std::istringstream>(objFile->getDataAsString());

    //    std::unique_ptr<std::istringstream> mtlStream = nullptr;
    //    std::unique_ptr<tinyobj::MaterialStreamReader> materialReader = nullptr;
    //    if(mtlFile) {
    //      mtlStream = std::make_unique<std::istringstream>(mtlFile->getDataAsString());
    //      materialReader = std::make_unique<tinyobj::MaterialStreamReader>(*mtlStream);
    //    }

    //    bool ret =
    //        tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objStream.get(),
    //        materialReader.get());

    //    if(!ret)
    //      SEQUOIA_THROW(GameException, "failed to load obj mesh {}: {}", objFile->getPath(), err);

    //    if(!err.empty())
    //      Log::warn("obj mesh: {}: {}", objFile->getPath(), err);

    //    // Append `default` material
    //    materials.push_back(tinyobj::material_t());

    //    math::AxisAlignedBox aab;

    //    // Allocate temporary buffer
    //    std::unordered_map<render::Vertex3D, std::size_t> indexMap;

    //    aligned_vector<unsigned int> indexBuffer;
    //    aligned_vector<render::Vertex3D> vertexBuffer;

    //    for(std::size_t s = 0; s < shapes.size(); ++s) {
    //      std::size_t numIndicesPerFace = shapes[s].mesh.indices.size() / 3;
    //      for(std::size_t f = 0; f < numIndicesPerFace; ++f) {

    //        // Vertices per face
    //        std::array<render::Vertex3D, 3> vertices;

    //        tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
    //        tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
    //        tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

    //        // Decode vertices
    //        for(int k = 0; k < 3; ++k) {
    //          int f0 = idx0.vertex_index;
    //          int f1 = idx1.vertex_index;
    //          int f2 = idx2.vertex_index;

    //          vertices[0].Position[k] = attrib.vertices[3 * f0 + k];
    //          vertices[1].Position[k] = attrib.vertices[3 * f1 + k];
    //          vertices[2].Position[k] = attrib.vertices[3 * f2 + k];
    //        }

    //        // Update bounding box
    //        for(int i = 0; i < 3; ++i)
    //          aab.merge(math::make_vec3(vertices[i].Position));

    //        // Decode normals
    //        if(attrib.normals.size() > 0) {
    //          int f0 = idx0.normal_index;
    //          int f1 = idx1.normal_index;
    //          int f2 = idx2.normal_index;

    //          for(int k = 0; k < 3; ++k) {
    //            vertices[0].Normal[k] = attrib.normals[3 * f0 + k];
    //            vertices[1].Normal[k] = attrib.normals[3 * f1 + k];
    //            vertices[2].Normal[k] = attrib.normals[3 * f2 + k];
    //          }
    //        } else {
    //          math::vec3 v0 = math::make_vec3(vertices[0].Position);
    //          math::vec3 v1 = math::make_vec3(vertices[1].Position);
    //          math::vec3 v2 = math::make_vec3(vertices[2].Position);

    //          math::vec3 v10 = v1 - v0;
    //          math::vec3 v20 = v2 - v0;

    //          math::vec3 n = math::normalize(math::cross(v10, v20));

    //          for(int k = 0; k < 3; ++k) {
    //            vertices[0].Normal[k] = n[k];
    //            vertices[1].Normal[k] = n[k];
    //            vertices[2].Normal[k] = n[k];
    //          }
    //        }

    //        // Decode texture coordiantes
    //        if(attrib.texcoords.size() > 0) {
    //          int f0 = idx0.texcoord_index;
    //          int f1 = idx1.texcoord_index;
    //          int f2 = idx2.texcoord_index;

    //          for(int k = 0; k < 2; ++k) {
    //            vertices[0].TexCoord[k] = attrib.texcoords[2 * f0 + k];
    //            vertices[1].TexCoord[k] = attrib.texcoords[2 * f1 + k];
    //            vertices[2].TexCoord[k] = attrib.texcoords[2 * f2 + k];
    //          }

    //          if(param.TexCoordInvertV) {
    //            vertices[0].TexCoord[1] = 1.0f - vertices[0].TexCoord[1];
    //            vertices[1].TexCoord[1] = 1.0f - vertices[1].TexCoord[1];
    //            vertices[2].TexCoord[1] = 1.0f - vertices[2].TexCoord[1];
    //          }

    //        } else {
    //          for(int k = 0; k < 2; ++k) {
    //            vertices[0].TexCoord[k] = 0.0f;
    //            vertices[1].TexCoord[k] = 0.0f;
    //            vertices[2].TexCoord[k] = 0.0f;
    //          }
    //        }

    //        // Combine normal and diffuse to get color.
    //        float normal_factor = 0.2;
    //        float diffuse_factor = 1 - normal_factor;

    //        int curMaterialID = shapes[s].mesh.material_ids[f];

    //        if(curMaterialID < 0 || curMaterialID >= static_cast<int>(materials.size()))
    //          // Invaid material ID. Use default material which is added to the last item in
    //          // `materials`
    //          curMaterialID = materials.size() - 1;

    //        float diffuse[3];
    //        for(int k = 0; k < 3; ++k)
    //          diffuse[k] = materials[curMaterialID].diffuse[k];

    //        constexpr auto maxRGBValue = std::numeric_limits<render::Vertex3D::ColorType>::max();

    //        for(int i = 0; i < 3; ++i) {
    //          math::vec3 color(vertices[i].Normal[0] * normal_factor + diffuse[0] *
    //          diffuse_factor,
    //                           vertices[i].Normal[1] * normal_factor + diffuse[1] *
    //                           diffuse_factor,
    //                           vertices[i].Normal[2] * normal_factor + diffuse[2] *
    //                           diffuse_factor);

    //          color = math::normalize(color);
    //          color = 0.5f * color + 0.5f;

    //          vertices[i].Color[0] = maxRGBValue * color[0];
    //          vertices[i].Color[1] = maxRGBValue * color[1];
    //          vertices[i].Color[2] = maxRGBValue * color[2];
    //        }
    //        vertices[0].Color[3] = vertices[1].Color[3] = vertices[2].Color[3] = maxRGBValue;

    //        // Add vertices to the buffer and compute indices
    //        for(int i = 0; i < 3; ++i) {
    //          auto indexMapIt = indexMap.find(vertices[i]);
    //          if(indexMapIt != indexMap.end()) {
    //            indexBuffer.emplace_back(indexMapIt->second);
    //          } else {
    //            vertexBuffer.emplace_back(vertices[i]);
    //            unsigned int index = vertexBuffer.size() - 1;
    //            indexBuffer.emplace_back(index);
    //            indexMap.emplace(vertices[i], index);
    //          }
    //        }
    //      }
    //    }

    //    // Upload data to the GPU
    //    render::VertexDataParameter vertexParam(render::VertexData::DM_Triangles,
    //                                            render::Vertex3D::getLayout(),
    //                                            vertexBuffer.size(),
    //                                            indexBuffer.size(), usage);
    //    vertexParam.IndexType = render::IndexBuffer::IT_UInt32;
    //    vertexParam.UseVertexShadowBuffer = true;
    //    vertexParam.UseIndexShadowBuffer = false;
    //    vertexData = Game::getSingleton().getRenderSystem()->createVertexData(vertexParam);

    //    vertexData->getVertexBuffer()->write(
    //        vertexBuffer.data(), 0, vertexBuffer.size() * vertexData->getLayout()->SizeOf, true);
    //    vertexData->writeIndex(indexBuffer.data(), indexBuffer.size());
    //    vertexData->setAxisAlignedBox(aab);

    //    // Register the data
    //    vertexDataMutex_.lock();
    //    vertexData_.emplace_back(vertexData);
    //    record->Index = vertexData_.size() - 1;
    //    vertexDataMutex_.unlock();
#endif
  }

  record->Mutex.unlock();

  // TODO: Copy for modifieable

  Log::debug("Successfully loaded obj mesh \"{}\" from \"{}\"", name, file->getPath());
  return std::make_shared<Mesh>(name, vertexData, modifiable);
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
using CubeVertexDataElementType = float;
static CubeVertexDataElementType CubeVertexData[] = {   
                                    0.5, 0.5, 0.5,   0, 0, 1,   0, 0, 0,   1, 1,      // v0 (front)
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

using CubeIndicesElementType = unsigned int;
static CubeIndicesElementType CubeIndices[] = {0,  1,  2,  2,  3,  0,   // front
                                               4,  5,  6,  6,  7,  4,   // right
                                               8,  9,  10, 10, 11, 8,   // top
                                               12, 13, 14, 14, 15, 12,  // left
                                               16, 17, 18, 18, 19, 16,  // bottom
                                               20, 21, 22, 22, 23, 20}; // back

// clang-format on

} // anonymous namespace

std::shared_ptr<Mesh> MeshManager::createCube(const std::string& name, bool modifiable,
                                              const MeshParameter& param,
                                              render::Buffer::UsageHint usage) {
  Log::debug("Creating cube mesh \"{}\" ...", name);

  std::shared_ptr<render::VertexData> vertexData = nullptr;
  VertexDataAccessRecord* record = nullptr;

  internal::CubeInfo info{param};

  cubeMutex_.lock();

  auto it = cubeMeshLookupMap_.find(info);
  if(it != cubeMeshLookupMap_.end())
    record = it->second.get();
  else
    record = cubeMeshLookupMap_.emplace(std::move(info), std::make_unique<VertexDataAccessRecord>())
                 .first->second.get();

  record->Mutex.lock();

  cubeMutex_.unlock();

  if(record->Index != -1) {
    vertexDataMutex_.lock_read();
    vertexData = vertexData_[record->Index];
    vertexDataMutex_.unlock();
  } else {
    std::size_t numVertices = 24;
    std::size_t numIndices = 36;

    render::VertexLayout2 layout = render::Vertex_posf3_norf3_texf2_colu4::getLayout();
    render::VertexDataParameter vertexParam(render::VertexData::DM_Triangles, layout, numVertices,
                                            numIndices, usage);
    
    vertexParam.IndexType = render::IndexBuffer::IT_UInt32;
    vertexParam.UseVertexShadowBuffer = true;
    vertexParam.UseIndexShadowBuffer = false;

    vertexData = Game::getSingleton().getRenderSystem()->createVertexData(vertexParam);
    
    // Fill VertexBuffer
    {
      render::BufferGuard guard(vertexData->getVertexBuffer(), render::Buffer::LO_Discard);
      Byte* vertexPtr = guard.getAsByte();
      
      render::VertexAdapter adapter(layout);
      for(std::size_t i = 0; i < numVertices; ++i, vertexPtr += layout.SizeOf) {
        adapter.clear();
    
        // Position
        adapter.setPosition(math::make_vec3(CubeVertexData + i * CubeVertexDataStride));
        
        // Nornal
        adapter.setNormal(math::make_vec3(CubeVertexData + i * CubeVertexDataStride + 3));

        // Color
        Color color;
        for(int j = 0; j < 3; ++j)
          color[j] = Color::Uint8Max * CubeVertexData[i * CubeVertexDataStride + 6 + j];
        adapter.setColor(color);
                     
        // TexCoord
        math::vec2 texCoord;
        texCoord[0] = CubeVertexData[i * CubeVertexDataStride + 9];
        texCoord[1] = param.TexCoordInvertV ? 1.0f - CubeVertexData[i * CubeVertexDataStride + 10]
                                            : CubeVertexData[i * CubeVertexDataStride + 10];
        adapter.setTexCoord(texCoord);
    
        adapter.copyTo(vertexPtr);
      }
    }
    
    // Fill IndexBuffer
    vertexData->getIndexBuffer()->write(CubeIndices, 0, sizeof(CubeIndicesElementType) * numIndices);
    
    // Set bounding box
    vertexData->setAxisAlignedBox(
        math::AxisAlignedBox(math::vec3(-0.5, -0.5, -0.5), math::vec3(0.5, 0.5, 0.5)));

    // Register the data
    vertexDataMutex_.lock();
    vertexData_.emplace_back(vertexData);
    record->Index = vertexData_.size() - 1;
    vertexDataMutex_.unlock();
  }

  record->Mutex.unlock();

  // TODO: Copy for modifieable

  Log::debug("Successfully created cube mesh \"{}\"", name);
  return std::make_shared<Mesh>(name, vertexData, modifiable);
}

//===------------------------------------------------------------------------------------------===//
//    Grid
//===------------------------------------------------------------------------------------------===//

std::shared_ptr<Mesh> MeshManager::createGrid(const std::string& name, unsigned int N,
                                              bool modifiable, const MeshParameter& param,
                                              render::Buffer::UsageHint usage) {
  return nullptr;

  Log::debug("Creating grid mesh \"{}\" ...", name);

  std::shared_ptr<render::VertexData> vertexData = nullptr;
  VertexDataAccessRecord* record = nullptr;

  internal::GridInfo info{N, param};

  gridMutex_.lock();

  auto it = gridMeshLookupMap_.find(info);
  if(it != gridMeshLookupMap_.end())
    record = it->second.get();
  else
    record = gridMeshLookupMap_.emplace(std::move(info), std::make_unique<VertexDataAccessRecord>())
                 .first->second.get();

  record->Mutex.lock();

  gridMutex_.unlock();

  if(record->Index != -1) {
    vertexDataMutex_.lock_read();
    vertexData = vertexData_[record->Index];
    vertexDataMutex_.unlock();
  } else {
    unsigned int numVertices = N * N;
    unsigned int numIndices = 6 * (N - 1) * (N - 1);

    render::VertexLayout2 layout = render::Vertex_posf3_norf3_texf2_colu4::getLayout();
    render::VertexDataParameter vertexParam(render::VertexData::DM_Triangles, layout, numVertices,
                                            numIndices, usage);

    vertexParam.IndexType = render::IndexBuffer::IT_UInt32;
    vertexParam.UseVertexShadowBuffer = true;
    vertexParam.UseIndexShadowBuffer = false;

    vertexData = Game::getSingleton().getRenderSystem()->createVertexData(vertexParam);

    // Fill VertexBuffer
    {
      render::BufferGuard guard(vertexData->getVertexBuffer(), render::Buffer::LO_Discard);
      Byte* vertexPtr = guard.getAsByte();

      render::VertexAdapter adapter(layout);
      const float dx = 1.0f / (N - 1);

      for(unsigned int i = 0; i < N; ++i) {
        for(unsigned int j = 0; j < N; ++j, vertexPtr += layout.SizeOf) {
          // Position
          math::vec3 pos;
          pos.x = dx * (i - 0.5f * (N - 1));
          pos.y = 0.0f;
          pos.z = dx * (j - 0.5f * (N - 1));
          adapter.setPosition(pos);

          // Normal
          adapter.setNormal(math::vec3(0, 1, 0));

          // Color
          adapter.setColor(Color{});

          // TexCoord
          float u = i;
          float v = j;

          math::vec2 texCoord;
          texCoord.x = u;
          texCoord.y = param.TexCoordInvertV ? 1.0f - v : v;
          adapter.setTexCoord(texCoord);

          adapter.copyTo(vertexPtr);
        }
      }
    }

    // Fill IndexBuffer
    //
    //    0 ------ 3      1. Triangle { 1, 0, 3}
    //    |        |      2. Triangle { 4, 1, 3}
    //    |        |
    //    1 ------ 4
    //
    //   index = [ 1, 0, 3, 4, 1, 3 ]
    //
    {
      render::BufferGuard guard(vertexData->getIndexBuffer(), render::Buffer::LO_Discard);
      unsigned int* indices = guard.getAs<unsigned int>();

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
    }

    // Register the data
    vertexDataMutex_.lock();
    vertexData_.emplace_back(vertexData);
    record->Index = vertexData_.size() - 1;
    vertexDataMutex_.unlock();
  }

  record->Mutex.unlock();

  // TODO: Copy for modifieable

  Log::debug("Successfully created grid mesh \"{}\"", name);
  return std::make_shared<Mesh>(name, vertexData, modifiable);
}

std::size_t MeshManager::getNumMeshes() const { return vertexData_.size(); }

void MeshManager::freeUnusedMeshes() {
  SEQUOIA_LOCK_GUARD(vertexDataMutex_);

  auto it = std::remove_if(
      vertexData_.begin(), vertexData_.end(),
      [](const std::shared_ptr<render::VertexData>& meshPtr) { return meshPtr.use_count() == 1; });

  if(it != vertexData_.end()) {
    Log::info("Removing {} unused meshes", std::distance(it, vertexData_.end()));
    vertexData_.erase(it, vertexData_.end());
  }
}

} // namespace game

} // namespace sequoia
