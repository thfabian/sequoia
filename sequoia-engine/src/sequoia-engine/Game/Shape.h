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

#ifndef SEQUOIA_ENGINE_GAME_SHAPE_H
#define SEQUOIA_ENGINE_GAME_SHAPE_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Game/Material.h"
#include "sequoia-engine/Game/Mesh.h"
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace sequoia {

namespace game {

/// @brief Shape represented by a mesh and material properties
/// @ingroup game
class SEQUOIA_API Shape : public NonCopyable {
  /// Name of the shape
  const std::string name_;

  /// Meshes of the Shape
  std::vector<std::shared_ptr<Mesh>> meshes_;

  /// Materials of the Shape
  std::vector<std::shared_ptr<Material>> materials_;

public:
  /// @brief Create the shape
  ///
  /// @note This should never be called manually, use `ShapeManager::create` instead.
  Shape(const std::string& name, std::vector<std::shared_ptr<Mesh>> meshes,
        std::vector<std::shared_ptr<Material>> materials);

  /// @brief Add mesh/material pair
  void add(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);

  /// @brief Iterate `func` over each mesh/material pair
  void forEach(std::function<void(const std::shared_ptr<Mesh>&, const std::shared_ptr<Material>&)>
                   func) const;

  /// @brief Get the material
  const std::vector<std::shared_ptr<Material>>& getMaterials() const { return materials_; }

  /// @brief Get the mesh
  const std::vector<std::shared_ptr<Mesh>>& getMeshes() const { return meshes_; }

  /// @brief Get the name of the mesh
  const std::string& getName() const noexcept { return name_; }

  /// @brief Convert to string
  std::string toString() const;
};

} // namespace game

} // namespace sequoia

#endif
