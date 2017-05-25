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

#ifndef SEQUOIA_GAME_MESH_H
#define SEQUOIA_GAME_MESH_H

#include "sequoia/Core/Byte.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Game/Export.h"
#include "sequoia/Math/AxisAlignedBox.h"
#include "sequoia/Render/VertexArrayObject.h"

namespace sequoia {

namespace game {

class MeshManager;

/// @brief Ressource holding the 3D mesh
/// @ingroup game
class SEQUOIA_GAME_API Mesh : public NonCopyable {
public:
  friend class MeshManager;

  /// @brief Data of the mesh, this is shared among all meshes loaded from the same file
  struct Data {
    /// Vertex data
    void* DataPtr;

    /// Number of vertices
    std::size_t NumVertices;

    /// Layout of the vertex
    const render::VertexLayout* Layout;

    /// Local bounding box volume
    math::AxisAlignedBox AAB;
  };

  /// @brief Create empty mesh
  Mesh(const std::string& name);

  /// @brief Get the local bounding box
  const math::AxisAlignedBox& getAAB() const;

  /// @brief Get number of vertices
  std::size_t getNumVertices() const;

  /// @brief Get the VertexArrayObject
  render::VertexArrayObject* getVAO() const;

private:
  /// Device vertex data
  std::unique_ptr<render::VertexArrayObject> vao_;

  /// Host vertex data
  std::shared_ptr<Data> data_;

  /// Name of the mesh
  std::string name_;
};

} // namespace game

} // namespace sequoia

#endif
