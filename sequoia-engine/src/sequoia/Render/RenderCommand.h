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

#ifndef SEQUOIA_RENDER_RENDERCOMMAND_H
#define SEQUOIA_RENDER_RENDERCOMMAND_H

#include "sequoia/Core/Export.h"
#include "sequoia/Render/RenderFwd.h"
#include <memory>
#include <string>

namespace sequoia {

namespace render {

/// @brief Instructions on how to render to a framebuffer
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
  void setGlobalRenderState(const std::shared_ptr<GlobalRenderState>& state) noexcept {
    globalRenderState_ = state;
  }

  /// @brief Get/Set the RenderTarget to render to
  RenderTarget* getRenderTarget() const noexcept { return target_; }
  void setRenderTarget(RenderTarget* target) noexcept { target_ = target; }

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
