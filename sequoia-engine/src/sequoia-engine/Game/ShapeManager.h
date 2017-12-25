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

#ifndef SEQUOIA_ENGINE_GAME_SHAPEMANAGER_H
#define SEQUOIA_ENGINE_GAME_SHAPEMANAGER_H

#include "sequoia-engine/Core/Buffer.h"
#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/File.h"
#include "sequoia-engine/Core/Hash.h"
#include "sequoia-engine/Core/Mutex.h"
#include "sequoia-engine/Game/Material.h"
#include "sequoia-engine/Game/Mesh.h"
#include "sequoia-engine/Game/Shape.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/Texture.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <unordered_map>
#include <vector>

namespace Assimp {
class Importer;
}

namespace sequoia {

namespace game {

namespace internal {

// struct MeshInfo {
//  std::shared_ptr<core::File> File;
//  MeshParameter Param;

//  bool operator==(const MeshInfo& other) const noexcept {
//    return *File == *other.File && Param == other.Param;
//  }
//};

struct CubeInfo {
  MeshParameter Param;

  bool operator==(const CubeInfo& other) const noexcept { return Param == other.Param; }
};

// struct GridInfo {
//  unsigned int N;
//  MeshParameter Param;

//  bool operator==(const GridInfo& other) const noexcept {
//    return N == other.N && Param == other.Param;
//  }
//};

} // namespace internal

} // namespace game

} // namespace sequoia

// SEQUOIA_DECLARE_STD_HASH(sequoia::game::internal::MeshInfo, value, value.File, value.Param)
SEQUOIA_DECLARE_STD_HASH(sequoia::game::internal::CubeInfo, value, value.Param)
// SEQUOIA_DECLARE_STD_HASH(sequoia::game::internal::GridInfo, value, value.N, value.Param)

namespace sequoia {

namespace game {

/// @brief Handle creation and allocation of @ref sequoia::game::Mesh "meshes"
/// @ingroup game
class SEQUOIA_API ShapeManager : public NonCopyable {
public:
  ShapeManager();
  ~ShapeManager();

  //  /// @brief Load mesh from disk
  //  ///
  //  /// @param name         Name of the mesh
  //  /// @param file         Path to the object file
  //  /// @param modifiable   Request a copy of the mesh which allows to modify the vertex data
  //  /// @param param        Parameter used to initialize the mesh
  //  /// @param usage        Buffer usage of the hardware vertex buffers
  //  ///
  //  /// @throws GameException   Unable to load the mesh (invalid format)
  //  ///
  //  /// @remark Thread-safe
  //  std::shared_ptr<Mesh> load(const std::string& name, const std::shared_ptr<File>& file,
  //                             bool modifiable = false, const MeshParameter& param =
  //                             MeshParameter(),
  //                             render::Buffer::UsageHint usage =
  //                             render::Buffer::UH_StaticWriteOnly);

  /// @brief Create a unit cube, centered at `(0, 0, 0)`, spanning
  /// `{-0.5, 0.5} x {-0.5, 0.5} x {-0.5, 0.5}`
  ///
  /// @param name         Name of the mesh
  /// @param modifiable   Request a copy of the mesh which allows to modify the vertex data
  /// @param param        Parameter used to initialize the mesh
  /// @param usage        Buffer usage of the hardware vertex buffers
  ///
  /// @verbatim
  ///    v6----- v5
  ///   /|      /|
  ///  v1------v0|
  ///  | |     | |
  ///  | |v7---|-|v4
  ///  |/      |/
  ///  v2------v3
  /// @endverbatim
  ///
  /// @remark Thread-safe
  std::shared_ptr<Shape>
  createCube(const std::string& name, bool modifiable = false,
             const MeshParameter& param = MeshParameter(),
             core::Buffer::UsageHint usage = core::Buffer::UH_StaticWriteOnly);

  //  /// @brief Create a grid, centered at `(0, 0, 0)`, with `N^2` gridpoints (vertices) spanning
  //  /// `{-0.5, 0.5} x {0} x {-0.5, 0.5}`
  //  ///
  //  /// Texture coordiantes are repeated i.e the texture should have an edge sampling of
  //  /// render::TextureParameter::EK_MirroredRepeat or render::TextureParameter::EK_MirroredRepeat
  //  ///
  //  /// Example for setting up a texture for the grid:
  //  ///
  //  /// @code{.cpp}
  //  ///   render::TextureParameter texParam;
  //  ///   texParam.Dim1EdgeSampling = texParam.Dim2EdgeSampling =
  //  ///       render::TextureParameter::EK_MirroredRepeat;
  //  ///   Game::getSingleton().createTexture("foo.png", texParam);
  //  /// @endcode
  //  ///
  //  /// @param name         Name of the mesh
  //  /// @param modifiable   Request a copy of the mesh which allows to modify the vertex data
  //  /// @param N            Number of gridpoints (vertices) in `X` and `Z` dimension
  //  /// @param param        Parameter used to initialize the mesh
  //  /// @param usage        Buffer usage of the hardware vertex buffers
  //  ///
  //  /// @verbatim
  //  ///      --------> X
  //  ///    |   _______
  //  ///    |  |_|_|_|_|
  //  ///    |  |_|_|_|_|  N = 5
  //  ///    v  |_|_|_|_|
  //  ///    Z  |_|_|_|_|
  //  ///
  //  /// @endverbatim
  //  ///
  //  /// @remark Thread-safe
  //  std::shared_ptr<Mesh>
  //  createGrid(const std::string& name, unsigned int N, bool modifiable = false,
  //             const MeshParameter& param = MeshParameter(),
  //             render::Buffer::UsageHint usage = render::Buffer::UH_StaticWriteOnly);

  /// @brief Get number of registered meshes
  std::size_t getNumShapes() const;

  /// @brief Free all unused shapes
  /// 
  /// @remark Thread-safe
  void freeUnusedShapes();

private:
  /// @brief Access record of the ShapeData
  ///
  /// This keeps track of the index into `shapes` and provides synchronization when modifiying the
  /// shape data during construction.
  struct ShapeAccessRecord : public NonCopyable {
    int Index = -1;    ///< Index of the vertex-data (-1 indicates invalid data)
    core::Mutex Mutex; ///< Access mutex for modifying the shape data
  };

  /// @brief Data of the Mesh and Material
  struct ShapeData {
    std::shared_ptr<render::VertexData> Data;
    std::unordered_map<int, std::shared_ptr<render::Texture>> Textures;
    std::unordered_map<std::string, render::UniformVariable> Uniforms;
  };

private:
  /// Assimp context
  std::shared_ptr<Assimp::Importer> importer_;
  Mutex importerMutex_;

  /// Record the shape data (use count of 1 implies the mesh is *not* in use)
  std::vector<std::unique_ptr<ShapeData>> shapeData_;
  ReadWriteMutex shapeDataMutex_;

  //  /// Loaded mesh record
  //  std::unordered_map<internal::MeshInfo, std::unique_ptr<AccessRecord>>
  //  meshLookupMap_;
  //  Mutex meshMutex_;

  /// Cube mesh record
  std::unordered_map<internal::CubeInfo, std::unique_ptr<ShapeAccessRecord>> cubeMeshLookupMap_;
  Mutex cubeMutex_;

  //  /// Grid mesh record
  //  std::unordered_map<internal::GridInfo, std::unique_ptr<AccessRecord>>
  //      gridMeshLookupMap_;
  //  Mutex gridMutex_;
};

} // namespace game

} // namespace sequoia

#endif
