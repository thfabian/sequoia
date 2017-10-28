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

#ifndef SEQUOIA_ENGINE_RENDER_GLOBALRENDERSTATE_H
#define SEQUOIA_ENGINE_RENDER_GLOBALRENDERSTATE_H

#include "sequoia-engine/Render/Program.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <string>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Global state of the render pipline
/// @ingroup render
class SEQUOIA_API GlobalRenderState {
public:
  GlobalRenderState() = default;
  GlobalRenderState(const GlobalRenderState&) = default;
  GlobalRenderState(GlobalRenderState&&) = default;

  GlobalRenderState& operator=(const GlobalRenderState&) = default;
  GlobalRenderState& operator=(GlobalRenderState&&) = default;

  /// @brief Set the uniform variable `name` to `value` of `progam`
  ///
  /// @param program    Program to apply the uniform variable
  /// @param name       Name of the uniform variable
  /// @param variable   Uniform variable to set
  void setPerProgramUniformVariable(Program* program, const std::string& name,
                                    const UniformVariable& value);

  /// @brief Set global uniform variable, shared by all programs, `name` to `value`
  ///
  /// @param name       Name of the uniform variable
  /// @param variable   Uniform variable to set
  void setSharedUniformVariable(const std::string& name, const UniformVariable& value);

  /// @brief Get the UniformVariable map exclusive to `program`
  /// @returns `NULL` if no no variables were registered for `program`
  const std::unordered_map<std::string, UniformVariable>*
  getPerProgramUniformVariables(Program* program) const noexcept {
    auto it = perProgramUniformVariables_.find(program);
    return (it != perProgramUniformVariables_.end() ? &it->second : nullptr);
  }

  /// @brief Get the UniformVariable map shared by *all* programs
  const std::unordered_map<std::string, UniformVariable>& getSharedUniformVariables() const
      noexcept {
    return sharedUniformVariables_;
  }

  /// @brief Check if `program` has exclusive UniformVariables registered
  bool hasPerProgramUniformVariables(Program* program) const noexcept {
    return perProgramUniformVariables_.count(program);
  }

  /// @brief Reset the state
  ///
  /// This clears all registered uniform variables.
  void reset();

  /// @brief Convert to string
  std::string toString() const;

private:
  /// Uniform variables shared by *all* programs
  std::unordered_map<std::string, UniformVariable> sharedUniformVariables_;

  /// Uniform variable per program
  std::unordered_map<Program*, std::unordered_map<std::string, UniformVariable>>
      perProgramUniformVariables_;
};

} // namespace render

} // namespace sequoia

#endif
