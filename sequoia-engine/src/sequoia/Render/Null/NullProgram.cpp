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

#include "sequoia/Render/Null/NullProgram.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/Shader.h"

namespace sequoia {

namespace render {

NullProgram::NullProgram(const std::set<std::shared_ptr<Shader>>& shaders)
    : Program(RK_Null), shaders_(shaders) {}

NullProgram::~NullProgram() {}

const std::set<std::shared_ptr<Shader>>& NullProgram::getShaders() const { return shaders_; }

std::string NullProgram::toString() const {
  return core::format(
      "NullProgram[\n"
      "  shaders = %s\n"
      "]",
      core::indent(core::toStringRange(shaders_, [](const std::shared_ptr<Shader>& shader) {
        return core::indent(shader->toString());
      })));
}

void NullProgram::makeValidImpl() {}

} // namespace render

} // namespace sequoia
