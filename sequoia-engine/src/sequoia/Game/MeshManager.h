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

#ifndef SEQUOIA_GAME_MESHMANAGER_H
#define SEQUOIA_GAME_MESHMANAGER_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/File.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Game/Mesh.h"
#include "sequoia/Render/Buffer.h"
#include "sequoia/Render/RenderFwd.h"
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace game {

/// @brief Handle creation and allocation of meshes
/// @ingroup game
class SEQUOIA_API MeshManager : public NonCopyable {

  /// Record of all the loaded meshes (use count of 1 implies the mesh is *not* in use)
  std::vector<std::shared_ptr<render::VertexData>> vertexDataList_;

  /// Cube mesh indices
  std::unordered_map<MeshParameter, std::size_t> cubeMeshLookupMap_;

  /// Plane mesh indices
  std::unordered_map<std::size_t, std::size_t> gridMeshLookupMap_;

public:
  /// @brief Load mesh from disk
  ///
  /// @param name         Name of the mesh
  /// @param file         Path to the mesh file
  /// @param modifiable   Request a copy of the mesh which allows to modify the vertex data
  /// @param usage        Buffer usage of the hardware vertex buffers
  std::shared_ptr<Mesh> load(const std::string& name, const std::shared_ptr<File>& file,
                             bool modifiable = false, const MeshParameter& param = MeshParameter(),
                             render::Buffer::UsageHint usage = render::Buffer::UH_StaticWriteOnly);

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
  std::shared_ptr<Mesh>
  createCube(const std::string& name, bool modifiable = false,
             const MeshParameter& param = MeshParameter(),
             render::Buffer::UsageHint usage = render::Buffer::UH_StaticWriteOnly);

  /// @brief Create a grid, centered at `(0, 0, 0)`, with `N^2` gridpoints (vertices) spanning
  /// `{-0.5, 0.5} x {0} x {-0.5, 0.5}`
  ///
  /// Texture coordiantes are repeated meaning the texture should have an
  /// render::TextureParameter::EK_MirroredRepeat or render::TextureParameter::EK_MirroredRepeat
  ///
  /// Example for setting up a texture for the grid:
  ///
  /// @code{.cpp}
  ///   render::TextureParameter texParam;
  ///   texParam.Dim1EdgeSampling = texParam.Dim2EdgeSampling =
  ///       render::TextureParameter::EK_MirroredRepeat;
  ///   Game::getSingleton().createTexture("foo.png", texParam);
  /// @endcode
  ///
  /// @param name         Name of the mesh
  /// @param modifiable   Request a copy of the mesh which allows to modify the vertex data
  /// @param N            Number of gridpoints (vertices) in `X` and `Z` dimension
  /// @param param        Parameter used to initialize the mesh
  /// @param usage        Buffer usage of the hardware vertex buffers
  ///
  /// @verbatim
  ///      --------> X
  ///    |   _______
  ///    |  |_|_|_|_|
  ///    |  |_|_|_|_|  N = 5
  ///    v  |_|_|_|_|
  ///    Z  |_|_|_|_|
  ///
  /// @endverbatim
  std::shared_ptr<Mesh>
  createGrid(const std::string& name, unsigned int N, bool modifiable = false,
             const MeshParameter& param = MeshParameter(),
             render::Buffer::UsageHint usage = render::Buffer::UH_StaticWriteOnly);

  /// @brief Get number of registered meshes
  std::size_t getNumMeshes() const;

  /// @brief Free all unused meshes
  void freeUnusedMeshes();
};

} // namespace game

} // namespace sequoia

#endif
