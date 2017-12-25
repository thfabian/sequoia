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
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Game/Material.h"

namespace sequoia {

namespace game {

Material::Material() {}

Material::Material(std::unordered_map<int, std::shared_ptr<render::Texture>> textures,
                   std::unordered_map<std::string, render::UniformVariable> uniforms)
    : textures_(std::move(textures)), uniforms_(std::move(uniforms)) {}

std::string Material::toString() const {
  return core::format(
      "Material[\n"
      "  textures = {},\n"
      "  uniforms = {}\n"
      "]",
      textures_.empty() ? "null" : core::indent(core::toStringRange(
                                       textures_,
                                       [](const auto& unitTexturePair) {
                                         return core::format(
                                             "texture = {{\n"
                                             "  unit = {},\n"
                                             "  texture = {}"
                                             "}}",
                                             unitTexturePair.first,
                                             core::indent(unitTexturePair.second->toString()));
                                       })),
      uniforms_.empty()
          ? "null"
          : core::indent(core::toStringRange(uniforms_, [](const auto& nameVariablePair) {
              return core::indent(core::format("uniform = {{\n"
                                               "  name = {},\n"
                                               "  variable = {}\n"
                                               "}}",
                                               nameVariablePair.first,
                                               core::indent(nameVariablePair.second.toString())));
            })));
}

} // namespace game

} // namespace sequoia
