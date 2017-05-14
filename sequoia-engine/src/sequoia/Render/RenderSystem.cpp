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

#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/RenderSystem.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(render::RenderSystem);

namespace render {

std::unique_ptr<RenderSystem> RenderSystem::create(RenderSystemKind kind) {
  switch(kind) {
  case RK_OpenGL:
    return std::make_unique<GLRenderSystem>();
  default:
    SEQUOIA_THROW(RenderSystemException, "invalid RenderSystem");
  }
  return nullptr;
}

void RenderSystem::setDebugMode(bool debugMode) { debugMode_ = debugMode; }

bool RenderSystem::debugMode() const { return debugMode_; }

RenderSystem::RenderSystem(RenderSystemKind kind) : RenderSystemObject(kind), debugMode_(false) {}

} // namespace render

} // namespace sequoia
