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

#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Game/SceneNode.h"
#include <algorithm>
#include <iostream>

namespace sequoia {

namespace game {

std::string SceneGraph::toString() const {
  return core::format("SceneGraph[\n"
                      " nodes = %s\n"
                      "]",
                      nodes_.size());
}

std::string SceneGraph::toDot() const { return std::string(); }

void SceneGraph::dump() const {
  for(const auto& node : nodes_)
    std::cout << node->toString() << "\n";
}

void SceneGraph::remove(const std::shared_ptr<SceneNode>& node) {
  nodes_.erase(std::remove(nodes_.begin(), nodes_.end(), node), nodes_.end());
}

void SceneGraph::update(const SceneNode::UpdateEvent& event) {
  apply([&event](SceneNode* node) { node->update(event); });
}

void SceneGraph::clear() { nodes_.clear(); }

std::size_t SceneGraph::size() const {
  std::size_t s = 0;
  apply([&s](SceneNode* node) { ++s; });
  return s;
}

} // namespace game

} // namespace sequoia
