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
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Game/Emittable.h"

namespace sequoia {

namespace game {

Emittable::~Emittable() {}

Emittable::Emittable(SceneNode* node, EmitterKind kind) : Base(node), kind_(kind), active_(true) {}

std::string Emittable::toString() const {
  auto stringPair = toStringImpl();
  return core::format("{}[\n  {}]", stringPair.first, core::indent(stringPair.second));
}

std::string Emittable::getUniformVariableName() const { return std::string("e_") + getName(); }

std::pair<std::string, std::string> Emittable::toStringImpl() const {
  return std::make_pair("Emittable", core::format("active = {},\n", active_ ? "true" : "false"));
}

} // namespace game

} // namespace sequoia
