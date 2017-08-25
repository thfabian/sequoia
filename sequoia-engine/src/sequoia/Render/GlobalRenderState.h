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

#ifndef SEQUOIA_RENDER_GLOBALRENDERSTATE_H
#define SEQUOIA_RENDER_GLOBALRENDERSTATE_H

#include "sequoia/Render/Program.h"
#include "sequoia/Render/UniformVariable.h"
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

  /// @brief Set the global uniform variable `name` to `value` of `progam`
  ///
  /// Global uniform variables are *persistent* across the different DrawCommands. However, the
  /// variables can be overwritten for indivudual DrawCommands via `DrawCommand::setUniformVariable`
  ///
  /// @param program    Program to apply the uniform variable
  /// @param variable   Uniform variable to set
  void setUniformVariable(const std::shared_ptr<Program>& program, const std::string& name,
                          const UniformVariable& value) noexcept {
    uniformVariables_[program][name] = value;
  }

  /// @brief Get the uniform variable map of `program`
  const std::unordered_map<std::string, UniformVariable>&
  getUniformVariables(const std::shared_ptr<Program>& program) const noexcept;

  /// @brief Check if a uniform variable map exists for `program`
  bool hasUniformVariables(const std::shared_ptr<Program>& program) const noexcept {
    return (uniformVariables_.find(program) != uniformVariables_.end());
  }

  /// @brief Convert to string
  std::string toString() const;

private:
  /// Presistent uniformvariables of the Program
  std::unordered_map<std::shared_ptr<Program>, std::unordered_map<std::string, UniformVariable>>
      uniformVariables_;
};

} // namespace render

} // namespace sequoia

#endif
