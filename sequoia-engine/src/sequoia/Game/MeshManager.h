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
#include <unordered_map>

namespace sequoia {

namespace game {

class SEQUOIA_GAME_API MeshManager : public NonCopyable {
  /// Record of all the loaded meshes
  std::vector<std::shared_ptr<Mesh::Data>> meshDataList_;

  /// Lookup map for path
  std::unordered_map<platform::String, std::size_t> pathLookupMap_;

public:
  using BufferUsageKind = render::VertexArrayObject::BufferUsageKind;

  /// @brief Load mesh from disk
  ///
  /// @param name     Name of the mesh
  /// @param path     Path to the mesh file
  std::shared_ptr<Mesh> load(const std::string& name, const platform::String& path,
                             BufferUsageKind usage = BufferUsageKind::BK_StaticWriteOnly);

  /// @brief Create a unit-cube mesh
  std::shared_ptr<Mesh> createCube(const std::string& name);
};

} // namespace game

} // namespace sequoia

#endif
