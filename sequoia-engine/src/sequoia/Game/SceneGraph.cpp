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

#include "sequoia/Core/Format.h"
#include "sequoia/Game/SceneGraph.h"
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
  auto stringify = [](SceneNode* node) { std::cout << node->toString() << "\n"; };
  apply(stringify);
}

void SceneGraph::clear() { nodes_.clear(); }

} // namespace game

} // namespace sequoia
