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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERCOMMAND_H
#define SEQUOIA_ENGINE_RENDER_RENDERCOMMAND_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/GlobalRenderState.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace sequoia {

namespace render {

/// @brief Instructions on how to render to a RenderTarget
/// @ingroup render
class SEQUOIA_API RenderCommand {
public:
  RenderCommand();
  RenderCommand(const RenderCommand&) = default;
  RenderCommand(RenderCommand&&) = default;

  RenderCommand& operator=(const RenderCommand&) = default;
  RenderCommand& operator=(RenderCommand&&) = default;

  /// @brief Get/Set the DrawCommandList to draw
  DrawCommandList* getDrawCommandList() const noexcept { return drawCommandList_.get(); }
  void setDrawCommandList(const std::shared_ptr<DrawCommandList>& list) noexcept {
    drawCommandList_ = list;
  }

  /// @brief Get/Set the DrawCommandList to draw
  GlobalRenderState* getGlobalRenderState() const noexcept { return globalRenderState_.get(); }

  /// @brief Get/Set the RenderTarget to render to
  RenderTarget* getRenderTarget() const noexcept { return target_; }
  void setRenderTarget(RenderTarget* target) noexcept { target_ = target; }

  /// @brief For each Program execute `functor` for each referenced UniformVariable for this Program
  ///
  /// @tparam Functor   Functor of type `void(Program*, const std::string&, const UniformVariable&)`
  ///
  /// @param programs   Programs to set the Uniformvariables
  /// @param functor    Functor to execture for each UniformVariable
  template <class Functor>
  void forEachUniformVariable(const std::unordered_set<Program*>& programs, Functor&& functor) {
    for(Program* program : programs) {
      // Set the per program uniform variables
      auto* perProgramUniformVariables = globalRenderState_->getPerProgramUniformVariables(program);
      if(perProgramUniformVariables)
        for(const auto& nameVariablePair : *perProgramUniformVariables)
          functor(program, nameVariablePair.first, nameVariablePair.second);

      // Set the shared uniform variables
      for(const auto& nameVariablePair : globalRenderState_->getSharedUniformVariables())
        functor(program, nameVariablePair.first, nameVariablePair.second);
    }
  }

  /// @brief Reset global GlobalRenderState and DrawCommandList
  void reset();

  /// @brief Convert draw command to string
  std::string toString() const;

private:
  /// List of draw commands to execute
  std::shared_ptr<DrawCommandList> drawCommandList_;

  /// Global state of the rendering pipline
  std::shared_ptr<GlobalRenderState> globalRenderState_;

  /// Target to render to
  render::RenderTarget* target_;
};

} // namespace render

} // namespace sequoia

#endif
