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

#ifndef SEQUOIA_GAME_SCENEGRAPH_H
#define SEQUOIA_GAME_SCENEGRAPH_H

#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Game/Export.h"
#include "sequoia/Game/SceneNode.h"

namespace sequoia {

namespace game {

/// @brief SceneGraphs hold all objects in the scene and are primarly used to stream data to all
/// nodes
/// @ingroup game
class SEQUOIA_GAME_API SceneGraph : public NonCopyable {

  /// Top-level nodes
  std::vector<std::shared_ptr<SceneNode>> nodes_;

public:
  /// @brief Insert a new scenen node
  void insert(const std::shared_ptr<SceneNode>& node) { nodes_.emplace_back(node); }

  /// @brief Create a new scene node of type `T` and pass `args...` to the constructor
  template <class T, class... Args>
  void create(Args&&... args) {
    nodes_.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
  }

  /// @brief Apply `functor` to each node of the graph
  void apply(const std::function<void(SceneNode*)>& functor) const {
    for(const auto& node : nodes_)
      node->apply(functor);
  }

  /// @brief Clear the graph
  void clear();

  /// @brief Convert the graph to string
  std::string toString() const;

  /// @brief Convert graph to dot
  std::string toDot() const;

  /// @brief Dump the graph to `stdout`
  void dump() const;
};

} // namespace game

} // namespace sequoia

#endif
