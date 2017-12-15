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

#ifndef SEQUOIA_ENGINE_GAME_MESH_H
#define SEQUOIA_ENGINE_GAME_MESH_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Hash.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Math/AxisAlignedBox.h"
#include "sequoia-engine/Render/VertexData.h"
#include <memory>

namespace sequoia {

namespace game {

/// @brief Parameters used to construct the Mesh
/// @ingroup game
struct SEQUOIA_API MeshParameter {

  /// Invert the v-component of the `uv` texture coordinates i.e `v = 1.0 - v`
  bool TexCoordInvertV = false;

  /// @name Constructor
  /// @{
  MeshParameter() = default;
  MeshParameter(const MeshParameter&) = default;
  MeshParameter(MeshParameter&&) = default;

  MeshParameter& operator=(const MeshParameter&) = default;
  MeshParameter& operator=(MeshParameter&&) = default;
  /// @}

  /// @name Comparison
  /// @{
  bool operator==(const MeshParameter& other) const noexcept;
  bool operator!=(const MeshParameter& other) const noexcept { return !(other == *this); }
  /// @}

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Ressource holding the 3D mesh
/// @ingroup game
class SEQUOIA_API Mesh : public NonCopyable {
  friend class MeshManager;

  /// Do we own the mesh i.e can we modify it?
  const bool isModifiable_;

  /// Vertex data
  std::shared_ptr<render::VertexData> data_;

  /// Name of the mesh
  std::string name_;

public:
  /// @brief Create empty mesh
  Mesh(const std::string& name, const std::shared_ptr<render::VertexData>& data, bool isModifiable);

  /// @brief Get the VertexData
  render::VertexData* getVertexData() const { return data_.get(); }

  /// @brief Get the axis aligned bounding box
  const math::AxisAlignedBox& getAxisAlignedBox() const noexcept;

  /// @brief Set the axis aligned bounding box
  void setAxisAlignedBox(const math::AxisAlignedBox& bbox);

  /// @brief Set the name of the mesh
  void setName(const std::string& name) { name_ = name; }

  /// @brief Get the name of the mesh
  const std::string& getName() const { return name_; }

  /// @brief Is the mesh modifyable?
  bool isModifiable() const { return isModifiable_; }

  /// @brief Dump the vertex data and indices to `stdout`
  /// @see render::VertexData::dump
  void dump() const;

  /// @brief Convert to string
  std::string toString() const;
};

} // namespace game

} // namespace sequoia

SEQUOIA_DECLARE_STD_HASH(sequoia::game::MeshParameter, param, param.TexCoordInvertV)
SEQUOIA_DECLARE_STD_HASH(std::shared_ptr<sequoia::game::MeshParameter>, paramPtr, *paramPtr)

#endif
