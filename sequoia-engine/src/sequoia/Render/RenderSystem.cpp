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

#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/RenderSystem.h"
#include "sequoia/Core/Unreachable.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(render::RenderSystem);

namespace render {

std::unique_ptr<RenderSystem> RenderSystem::create(RenderSystemKind kind) {
  switch(kind) {
  case RK_OpenGL:
    return std::make_unique<GLRenderSystem>();
  }
  sequoia_unreachable("invalid RenderSystem");
}

RenderSystem::RenderSystemKind RenderSystem::getKind() const { return kind_; }

RenderSystem::RenderSystem(RenderSystem::RenderSystemKind kind) : kind_(kind) {}

} // namespace render

} // namespace sequoia
