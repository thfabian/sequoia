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

#include "sequoia-engine/Render/GlobalRenderState.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"

namespace sequoia {

namespace render {

void GlobalRenderState::setPerProgramUniformVariable(Program* program, const std::string& name,
                                                     const UniformVariable& value) {
  perProgramUniformVariables_[program][name] = value;
}

void GlobalRenderState::setSharedUniformVariable(const std::string& name,
                                                 const UniformVariable& value) {
  sharedUniformVariables_[name] = value;
}

void GlobalRenderState::reset() {
  sharedUniformVariables_.clear();
  perProgramUniformVariables_.clear();
}

std::string GlobalRenderState::toString() const {
  auto varMapToString = [](const auto& varMap) {
    return core::indent(core::toStringRange(varMap, [](const auto& nameVarPair) {
      return core::format("name = {},\n"
                          "  variable = {}",
                          nameVarPair.first, core::indent(nameVarPair.second.toString()));
    }));
  };

  return core::format(
      "GlobalRenderState[\n"
      "  sharedUniformVariables = {},\n"
      "  perProgramUniformVariables = {}\n"
      "]",
      varMapToString(sharedUniformVariables_),
      core::indent(core::toStringRange(
          perProgramUniformVariables_, [&varMapToString](const auto& programVarMapPair) {
            return core::format("program = {},\n"
                                "  variables = {}",
                                core::indent(programVarMapPair.first->toString()),
                                varMapToString(programVarMapPair.second));
          })));
}

} // namespace render

} // namespace sequoia
