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

#include "sequoia/Render/RenderCommand.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/DrawCommandList.h"
#include "sequoia/Render/GlobalRenderState.h"
#include "sequoia/Render/RenderTarget.h"

namespace sequoia {

namespace render {

RenderCommand::RenderCommand()
    : drawCommandList_(nullptr), globalRenderState_(nullptr), target_(nullptr) {}

std::string RenderCommand::toString() const {
  return core::format(
      "RenderCommand[\n"
      "  numDrawCommands = %s,\n"
      "  globalRenderState = %s,\n"
      "  target = %s\n"
      "]",
      core::indent(drawCommandList_ ? std::to_string(drawCommandList_->size()) : "null"),
      core::indent(globalRenderState_ ? globalRenderState_->toString() : "null"),
      core::indent(target_ ? target_->toString() : "null"));
}

} // namespace render

} // namespace sequoia
