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

#ifndef SEQUOIA_GAME_MESHMANAGER_H
#define SEQUOIA_GAME_MESHMANAGER_H

#include "sequoia/Core/Platform.h"
#include "sequoia/Game/Export.h"
#include "sequoia/Game/Mesh.h"
#include "sequoia/Render/RenderFwd.h"
#include "sequoia/Render/VertexArrayObject.h"
#include <unordered_map>

namespace sequoia {

namespace game {

class SEQUOIA_GAME_API MeshManager : public NonCopyable {
  /// Record of all the loaded meshes (use count of 1 implies the mesh is *not* in use)
  std::vector<std::shared_ptr<render::VertexData>> vertexDataList_;

  /// Lookup map for path
  std::unordered_map<platform::String, std::size_t> pathLookupMap_;

  /// Index into `meshDataList` for the static cube mesh
  int staticCubeMeshDataIdx_;

public:
  using BufferUsageKind = render::VertexArrayObject::BufferUsageKind;

  MeshManager();

  /// @brief Load mesh from disk
  ///
  /// @param target       RenderTarget used to allocate the hardware buffers
  /// @param name         Name of the mesh
  /// @param path         Path to the mesh file
  /// @param copy         Request a copy of the mesh which allows to modify the vertex data
  /// @param usage        Buffer usage of the hardware vertex buffers
  std::shared_ptr<Mesh> load(render::RenderTarget* target, const std::string& name,
                             const platform::String& path, bool copy = false,
                             BufferUsageKind usage = BufferUsageKind::BK_StaticWriteOnly);

  /// @brief Create a cube mesh which is centered at `(0, 0, 0)` and spans
  /// `{-1, 1} x {-1, 1} x {-1, 1}`
  ///
  /// @param target       RenderTarget used to allocate the hardware buffers
  /// @param name         Name of the mesh
  /// @param copy         Request a copy of the mesh which allows to modify the vertex data
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
  std::shared_ptr<Mesh> createCube(render::RenderTarget* target, const std::string& name,
                                   bool copy = false,
                                   BufferUsageKind usage = BufferUsageKind::BK_StaticWriteOnly);
};

} // namespace game

} // namespace sequoia

#endif
