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
#include "sequoia-engine/Math/MathFwd.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include <memory>
#include <string>

namespace sequoia {

namespace game {

/// @brief Parameters used to construct the Mesh
/// @ingroup game
struct SEQUOIA_API MeshParameter {

  /// Invert the v-component of the `uv` texture coordinates i.e `v = 1.0 - v`
  bool TexCoordInvertV = false;

  /// @name Comparison
  /// @{
  bool operator==(const MeshParameter& other) const noexcept;
  bool operator!=(const MeshParameter& other) const noexcept { return !(other == *this); }
  /// @}

  /// @brief Convert to string
  std::string toString() const;
};

/// @brief Mesh of a Shape
/// @ingroup game
class SEQUOIA_API Mesh : public NonCopyable {
  friend class MeshManager;

  /// Do we own the mesh i.e can we modify it?
  const bool modifiable_;

  /// Underlying data store
  std::shared_ptr<render::VertexData> data_;

public:
  /// @brief Create the mesh
  ///
  /// @note This should never be called manually, use `ShapeManager::load` instead.
  Mesh(const std::shared_ptr<render::VertexData>& data, bool isModifiable);

  /// @brief Get the VertexData
  render::VertexData* getVertexData() const noexcept { return data_.get(); }

  /// @brief Get the axis aligned bounding box
  const math::AxisAlignedBox& getAxisAlignedBox() const noexcept;

  /// @brief Is the mesh modifyable?
  bool isModifiable() const noexcept { return modifiable_; }

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
