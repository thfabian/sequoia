//===---------------------------------------------------------------------------------*- C++-*-===//
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

#include "sequoia-engine/Game/SceneNode.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringRef.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Math/CoordinateSystem.h"
#include <boost/lexical_cast.hpp>
#include <numeric>

namespace sequoia {

namespace game {

SceneNode::SceneNode(const std::string& name, SceneNode::SceneNodeKind kind)
    : std::enable_shared_from_this<SceneNode>(), kind_(kind), position_(math::vec3()),
      orientation_(math::quat()), scale_(1.0f), modelMatrix_(), modelMatrixIsDirty_(true),
      parent_(), name_(name) {
  for(int i = 0; i < capabilities_.size(); ++i)
    capabilities_[i] = nullptr;
}

SceneNode::SceneNode(const SceneNode& other)
    : std::enable_shared_from_this<SceneNode>(), kind_(other.kind_), position_(other.position_),
      orientation_(other.orientation_), scale_(other.scale_), modelMatrix_(other.modelMatrix_),
      modelMatrixIsDirty_(other.modelMatrixIsDirty_), parent_(other.parent_) {

  // Adjust the name, we append a `_copy_X` where `X` is the version of the copy
  StringRef nameRef(other.name_);
  std::size_t copyPos = nameRef.find("_copy_");

  if(copyPos != StringRef::npos) {
    StringRef versionStr = nameRef.substr(copyPos + sizeof("_copy_") - 1);
    int version = boost::lexical_cast<int>(versionStr.data(), versionStr.size()) + 1;
    name_ = nameRef.substr(0, copyPos).str() + "_copy_" + std::to_string(version);
  } else {
    name_ = other.name_ + "_copy_1";
  }

  // Clone children
  for(const auto& child : other.children_)
    children_.emplace_back(child->clone());

  // Clone capabilites
  for(int i = 0; i < capabilities_.size(); ++i)
    capabilities_[i] = other.capabilities_[i] ? other.capabilities_[i]->clone(this) : nullptr;
}

SceneNode::~SceneNode() {}

void SceneNode::resetOrientation() { setOrientation(math::quat()); }

math::mat3 SceneNode::getLocalAxes() const {
  math::vec3 axisX = getOrientation() * math::CoordinateSystem::X();
  math::vec3 axisY = getOrientation() * math::CoordinateSystem::Y();
  math::vec3 axisZ = getOrientation() * math::CoordinateSystem::Z();
  return math::mat3(axisX, axisY, axisZ);
}

void SceneNode::applyImpl(const std::function<void(SceneNode*)>& functor) {
  functor(this);
  for(const auto& child : getChildren())
    child->applyImpl(functor);
}

void SceneNode::applyNoexceptImpl(
    const std::function<void(SceneNode*) noexcept>& functor) noexcept {
  functor(this);
  for(const auto& child : getChildren())
    child->applyNoexceptImpl(functor);
}

void SceneNode::update(const UpdateEvent& event) {
  for(int i = 0; i < capabilities_.size(); ++i)
    if(capabilities_[i])
      capabilities_[i]->update(event);
}

void SceneNode::translate(const math::vec3& d, TransformSpace relativeTo) {
  setPosition(getPosition() + d);
}

void SceneNode::rotate(const math::quat& q, TransformSpace relativeTo) {
  math::quat newOrientation = q * getOrientation();

  // Normalize quaternion to avoid drift
  newOrientation = math::normalize(newOrientation);
  setOrientation(newOrientation);
}

void SceneNode::rotate(const math::vec3& axis, const math::Radian& angle,
                       SceneNode::TransformSpace relativeTo) {
  rotate(math::angleAxis(angle.inRadians(), axis), relativeTo);
}

void SceneNode::roll(const math::Radian& angle, TransformSpace relativeTo) {
  rotate(math::CoordinateSystem::Z(), angle, relativeTo);
}

void SceneNode::pitch(const math::Radian& angle, TransformSpace relativeTo) {
  rotate(math::CoordinateSystem::X(), angle, relativeTo);
}

void SceneNode::yaw(const math::Radian& angle, TransformSpace relativeTo) {
  rotate(math::CoordinateSystem::Y(), angle, relativeTo);
}

std::shared_ptr<SceneNode> SceneNode::clone() { return SceneNode::allocate<SceneNode>(*this); }

std::string SceneNode::toString() const {
  auto stringPair = toStringImpl();
  return core::format("{}[\n  {}]", stringPair.first, core::indent(stringPair.second));
}

std::pair<std::string, std::string> SceneNode::toStringImpl() const {
  return std::make_pair(
      "SceneNode",
      core::format(
          "name = {},\n"
          "position = {},\n"
          "orientation = {},\n"
          "scale = {},\n"
          "parent = {},\n"
          "capabilities = {},\n"
          "children = {},\n",
          name_, position_, orientation_, scale_, hasParent() ? getParent()->getName() : "null",

          // Capabilities
          std::accumulate(
              capabilities_.begin(), capabilities_.end(), true,
              [](bool isNull, const auto& capability) { return isNull && capability == nullptr; })
              ? "null"
              : core::toStringRange(capabilities_,
                                    [](const auto& capability) {
                                      return core::indent(capability ? capability->toString()
                                                                     : "null");
                                    }),

          // Children
          hasChildren()
              ? core::toStringRange(children_,
                                    [](const auto& node) { return core::indent(node->toString()); })
              : "null"));
}

void SceneNode::computeModelMatrix() {
  // ModelMatrix = TranslationMatrix * RotatationMatrix * ScaleMatrix
  modelMatrix_ = math::translate(math::mat4(1.0f), position_);
  modelMatrix_ = modelMatrix_ * math::mat4_cast(orientation_);
  modelMatrix_ = math::scale(modelMatrix_, glm::vec3(scale_));

  modelMatrixIsDirty_ = false;
}

} // namespace game

} // namespace sequoia
