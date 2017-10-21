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

#include "sequoia/Core/Format.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Game/SceneNode.h"
#include <algorithm>
#include <atomic>
#include <iostream>

namespace sequoia {

namespace game {

SceneGraph::SceneGraph() { root_ = SceneNode::allocate<SceneNode>("__root__"); }

SceneGraph::~SceneGraph() {}

void SceneGraph::update(const SceneNode::UpdateEvent& event) {
  // TODO: Maybe do in parallel?
  apply([&event](SceneNode* node) { node->update(event); });
}

void SceneGraph::clear() { root_->clearChildren(); }

std::size_t SceneGraph::size() const {
  std::atomic<std::size_t> s{0};
  apply([&s](SceneNode * node) noexcept { s++; }, SceneNode::EP_Parallel);
  return (s.load() - 1);
}

std::string SceneGraph::toString() const {
  return core::format("SceneGraph[\n"
                      "  nodes = {}\n"
                      "]",
                      size());
}

std::string SceneGraph::toDot() const { return std::string(); }

void SceneGraph::dump() const {
  apply([](SceneNode* node) { std::cout << node->toString() << "\n"; }, SceneNode::EP_Sequential);
}

} // namespace game

} // namespace sequoia
