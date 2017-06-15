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

#include "sequoia/Core/File.h"
#include "sequoia/Core/Platform.h"
#include "sequoia/Game/Export.h"
#include "sequoia/Game/Mesh.h"
#include "sequoia/Render/RenderFwd.h"
#include "sequoia/Render/VertexArrayObject.h"
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace game {

/// @brief Handle creation of meshes of a specifc `RenderTarget`
/// @ingroup game
class SEQUOIA_API MeshManager : public NonCopyable {

  /// Target used to create device buffers
  render::RenderTarget* target_;

  /// Record of all the loaded meshes (use count of 1 implies the mesh is *not* in use)
  std::vector<std::shared_ptr<render::VertexData>> vertexDataList_;

  /// Indices for the static meshes
  int staticCubeMeshDataIdx_;
  int staticTriangleMeshDataIdx_;

public:
  using BufferUsageKind = render::VertexArrayObject::BufferUsageKind;

  /// @brief Crate mesh-manager for a specific `RenderTarget`
  MeshManager(render::RenderTarget* target);

  /// @brief Load mesh from disk
  ///
  /// @param name         Name of the mesh
  /// @param file         Path to the mesh file
  /// @param modifiable   Request a copy of the mesh which allows to modify the vertex data
  /// @param usage        Buffer usage of the hardware vertex buffers
  std::shared_ptr<Mesh> load(const std::string& name, const std::shared_ptr<File>& file,
                             bool modifiable = false,
                             BufferUsageKind usage = BufferUsageKind::BK_StaticWriteOnly);

  /// @brief Create a cube mesh which is centered at `(0, 0, 0)` and spans
  /// `{-1, 1} x {-1, 1} x {-1, 1}`
  ///
  /// @param name         Name of the mesh
  /// @param modifiable   Request a copy of the mesh which allows to modify the vertex data
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
  std::shared_ptr<Mesh> createCube(const std::string& name, bool modifieable = false,
                                   BufferUsageKind usage = BufferUsageKind::BK_StaticWriteOnly);
};

} // namespace game

} // namespace sequoia

#endif
