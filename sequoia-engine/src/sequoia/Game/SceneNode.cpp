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
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Game/SceneNode.h"

namespace sequoia {

namespace game {

SceneNode::SceneNode(SceneNode::SceneNodeKind kind, const std::shared_ptr<SceneNode>& parent,
                     const std::string& name, const glm::vec3& position,
                     const glm::quat& orientation, float scale)
    : kind_(kind), mesh_(nullptr), position_(position), orientation_(orientation), scale_(scale), 
      parent_(parent), name_(name) {
  LOG(DEBUG) << "Creating ScenenNode \"" << name_ << "\"";
}

SceneNode::~SceneNode() { LOG(DEBUG) << "Deleting ScenenNode \"" << name_ << "\""; }

void SceneNode::apply(const std::function<void(SceneNode*)>& functor) {
  functor(this);
  for(const auto& child : children_)
    child->apply(functor);
}

std::string SceneNode::toString() const {
  auto stringPair = toStringImpl();
  return core::format("%s[\n%s\n]", stringPair.first, core::indent(stringPair.second));
}

} // namespace game

} // namespace sequoia
