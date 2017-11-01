//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia-engine/Render/RenderSystem.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/Exception.h"

#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/Null/NullRenderSystem.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(render::RenderSystem);

namespace render {

std::unique_ptr<RenderSystem> RenderSystem::create(RenderSystemKind kind, Options* options) {
  switch(kind) {
  case RK_OpenGL:
    return std::make_unique<GLRenderSystem>(options);
  case RK_Null:
    return std::make_unique<NullRenderSystem>(options);
  default:
    SEQUOIA_THROW(RenderSystemException, "RenderSystem not avaialable");
  }
  return nullptr;
}

RenderSystem::RenderSystem(RenderSystemKind kind, Options* options)
    : RenderSystemObject(kind), options_(options) {
  SEQUOIA_ASSERT_MSG(options_, "invalid options");
}

RenderSystem::~RenderSystem() {}

void RenderSystem::frameListenerRenderingBegin(RenderCommand* command) {
  for(FrameListener* listener : getListeners<FrameListener>())
    listener->frameListenerRenderingBegin(command);
}

void RenderSystem::frameListenerRenderingEnd(RenderCommand* command) {
  for(FrameListener* listener : getListeners<FrameListener>())
    listener->frameListenerRenderingEnd(command);
}

} // namespace render

} // namespace sequoia