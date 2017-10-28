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

#ifndef SEQUOIA_ENGINE_GAME_SCENEGRAPH_H
#define SEQUOIA_ENGINE_GAME_SCENEGRAPH_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Game/SceneNode.h"
#include <memory>
#include <vector>

namespace sequoia {

namespace game {

/// @brief SceneGraphs hold all objects in the scene and are primarly used to stream data to all
/// nodes
/// @ingroup game
class SEQUOIA_API SceneGraph : public NonCopyable {
  /// Root node
  std::shared_ptr<SceneNode> root_;

public:
  SceneGraph();
  ~SceneGraph();

  /// @brief Insert a new scenen `node` into the graph
  void insert(const std::shared_ptr<SceneNode>& node) { root_->addChild(node); }

  /// @brief Remove the `node` (and all its children) from the graph
  void remove(const std::shared_ptr<SceneNode>& node) { root_->removeChild(node); }

  /// @brief Update all nodes to indicate we moved on to the next time-step
  void update(const SceneNode::UpdateEvent& event);

  /// @brief Apply `functor` to the node and all its children
  ///
  /// @tparam Functor   Function type: `void(SceneNode*)` or `void(SceneNode*) noexcept`
  /// @param functor    Functor to apply to the node and its children
  /// @param policy     Executation policy to launch `functor`
  template <class Functor>
  void apply(Functor&& functor,
             SceneNode::ExecutionPolicy policy = SceneNode::EP_Sequential) const {
    root_->apply(std::forward<Functor>(functor), policy);
  }

  /// @brief Clear the graph
  void clear();

  /// @brief Get the number of nodes in the graph
  std::size_t size() const;

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
