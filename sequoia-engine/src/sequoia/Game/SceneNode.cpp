//===-/-------------------------------------------------------------------------------*- C++ -*-===//
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
#include "sequoia/Game/SceneGraph.h"

namespace sequoia {

namespace game {

SceneNode::SceneNode(SceneNode::SceneNodeKind kind, const std::string& name)
    : kind_(kind), mesh_(nullptr), position_(math::vec3()), orientation_(math::quat()), scale_(1.0f), modelMatrix_(), modelMatrixIsDirty_(true),
      parent_(), name_(name) {
  LOG(DEBUG) << "Creating ScenenNode \"" << name_ << "\"";
}

SceneNode::SceneNode(const SceneNode & other)
  :   kind_(other.kind_), mesh_(other.mesh_), position_(other.position_), orientation_(other.orientation_), scale_(other.scale_), 
      modelMatrix_(other.modelMatrix_), modelMatrixIsDirty_(other.modelMatrixIsDirty_),
      parent_(other.parent_), name_(other.name_) {
  for(const auto& child : other.children_)
    children_.emplace_back(child->clone());
}

SceneNode::~SceneNode() { LOG(DEBUG) << "Deleting ScenenNode \"" << name_ << "\""; }

void SceneNode::apply(const std::function<void(SceneNode*)>& functor) {
  functor(this);
  for(const auto& child : children_)
    child->apply(functor);
}

std::shared_ptr<SceneNode> SceneNode::clone() { return SceneGraph::create<SceneNode>(*this); }

std::string SceneNode::toString() const {
  auto stringPair = toStringImpl();
  return core::format("%s[\n%s\n]",  stringPair.first, core::indent(stringPair.second));
}

std::pair<std::string, std::string> SceneNode::toStringImpl() const
{
  return std::make_pair("SceneNode",
                        core::format("  name = %s,\n"
                                     "  mesh = %s,\n"
                                     "  position = %s,\n"
                                     "  orientation = %s,\n"
                                     "  scale = %f,\n"
                                     "  parent = %s,\n"
                                     "  children = %s,\n", 
                                     name_, core::indent(mesh_->toString()), position_, 
                                     orientation_, scale_, 
                                     hasParent() ? getParent()->toString() : "null", 
                                     hasChildren() ? 
                                        core::indent(core::toStringRange(children_, 
                        [](const auto& node) { return core::indent(node->toString()); })) :  "null")
  );
}

void SceneNode::computeModelMatrix()
{
  // ModelMatrix = TranslationMatrix * RotatationMatrix * ScaleMatrix
  modelMatrix_ = math::mat4(1.0f);
  modelMatrix_ = math::translate(modelMatrix_, position_);
  modelMatrix_ = modelMatrix_ * math::mat4_cast(orientation_);
  modelMatrix_ = math::scale(modelMatrix_, glm::vec3(scale_));
  
  modelMatrixIsDirty_ = false;
}

} // namespace game

} // namespace sequoia
