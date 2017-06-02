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

#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Render/DrawCommand.h"
#include "sequoia/Render/Program.h"
#include "sequoia/Render/VertexArrayObject.h"

namespace sequoia {

namespace render {

static const char* depthFuncToString(RenderState::DepthFuncKind func) {
  switch(func) {
  case RenderState::DepthFuncKind::DF_Never:
    return "Never";
  case RenderState::DepthFuncKind::DF_Less:
    return "Less";
  case RenderState::DepthFuncKind::DF_Equal:
    return "Equal";
  case RenderState::DepthFuncKind::DF_LessEqual:
    return "LessEqual";
  case RenderState::DepthFuncKind::DF_Greater:
    return "Greater";
  case RenderState::DepthFuncKind::DF_NotEqual:
    return "NotEqual";
  case RenderState::DepthFuncKind::DF_GreaterEqual:
    return "GreaterEqual";
  case RenderState::DepthFuncKind::DF_Always:
    return "Always";
  default:
    sequoia_unreachable("invalid DepthFuncKind");
  }
}

static const char* drawModeToString(RenderState::DrawModeKind mode) {
  switch(mode) {
  case RenderState::DrawModeKind::DM_Triangles:
    return "Triangles";
  default:
    sequoia_unreachable("invalid DrawModeKind");
  }
}

std::string RenderState::toString() const {
  return core::format("RenderState["
                      "  DepthTest=%s,\n"
                      "  DepthFunc=%s,\n"
                      "  DrawMode=%s\n"
                      "]",
                      DepthTest, depthFuncToString(DepthFunc), drawModeToString(DrawMode));
}

std::string DrawCommand::toString() const {
  return core::format("DrawCommand["
                      "  program=%s,\n"
                      "  vao=%s,\n"
                      "  renderState=%s,\n"
                      "]",
                      program_->toString(), vao_->toString(), core::indent(state_->toString()));
}

} // namespace render

} // namespace sequoia
