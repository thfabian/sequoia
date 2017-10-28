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

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Game/PointLight.h"
#include "sequoia-engine/Game/SceneNode.h"
#include "sequoia-engine/Game/SceneNodeAlloc.h"
#include "sequoia-engine/Render/UniformStruct.h"

namespace sequoia {

namespace game {

namespace {

SEQUOIA_UNIFORM_STRUCT(PointLightStruct,
                       (math::vec3, Position)(math::vec3, Power)(math::vec4, Color));

} // anonymous namespace

PointLight::~PointLight() {}

PointLight::PointLight(SceneNode* node, math::vec3 power, Color color)
    : Base(node, EK_PointLight), power_(power), color_(color) {}

void PointLight::toUniformVariableMap(std::unordered_map<std::string, UniformVariable>& map,
                                      int index) {
  auto normalizedColor = color_.normalize();
  math::vec4 color = math::make_vec4(normalizedColor.data());

  PointLightStruct light{getNode()->getPosition(), power_, color};
  light.toUniformVariableMap(getUniformVariableName(), map, index);
}

const char* PointLight::getName() const { return "PointLight"; }

void PointLight::update(const SceneNodeUpdateEvent& event) {}

std::shared_ptr<SceneNodeCapability> PointLight::clone(SceneNode* node) const {
  return scene::allocate_shared<PointLight>(node, power_, color_);
}

std::pair<std::string, std::string> PointLight::toStringImpl() const {
  return std::make_pair(getName(), core::format("{}"
                                                "power = {},\n"
                                                "color = {}\n",
                                                Base::toStringImpl().second, power_, color_));
}

} // namespace game

} // namespace sequoia
