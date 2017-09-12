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

#include "sequoia/Render/GlobalRenderState.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"

namespace sequoia {

namespace render {

const std::unordered_map<std::string, UniformVariable>&
GlobalRenderState::getUniformVariables(const std::shared_ptr<Program>& program) const noexcept {
  auto it = uniformVariables_.find(program);
  SEQUOIA_ASSERT(it != uniformVariables_.end());
  return it->second;
}

std::string GlobalRenderState::toString() const {
  return core::format(
      "GlobalRenderState[\n"
      "  uniformVariables = %s\n"
      "]",
      core::indent(core::toStringRange(uniformVariables_, [](const auto& programVarMapPair) {
        return core::format(
            "program = %s,\n"
            "  variables = %s",
            core::indent(programVarMapPair.first->toString()),
            core::indent(
                core::toStringRange(programVarMapPair.second, [](const auto& nameUniformVariable) {
                  return core::format("name = %s,\n"
                                      "  variable = %s",
                                      nameUniformVariable.first,
                                      core::indent(nameUniformVariable.second.toString()));
                })));
      })));
}

} // namespace render

} // namespace sequoia
