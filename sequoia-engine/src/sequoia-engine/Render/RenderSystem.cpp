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

#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/RenderSystem.h"

#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/Null/NullRenderSystem.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(render::RenderSystem);

namespace render {

std::shared_ptr<Options> RenderSystem::makeOptions() {
  auto options = std::make_shared<Options>();
  RenderSystem::setDefaultOptions(options);
  return options;
}

std::unique_ptr<RenderSystem> RenderSystem::create(RenderSystemKind kind,
                                                   const std::shared_ptr<Options>& options) {
  RenderSystem::setDefaultOptions(options);

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

RenderSystem::RenderSystem(RenderSystemKind kind, const std::shared_ptr<Options>& options)
    : RenderSystemObject(kind), options_(options) {
  SEQUOIA_ASSERT_MSG(options_, "invalid options");
}

void RenderSystem::setDefaultOptions(const std::shared_ptr<Options>& options) {
  // Core
  core::setDefaultOptions(options);

  // Shared
  options->setDefaultString("Render.WindowMode", "window",
                            OptionMetaData{"window-mode", "", true, "MODE",
                                           "Set the window mode to MODE, where MODE is one of "
                                           "[window,fullscreen,windowed-fullscreen]"});
  options->setDefaultInt("Render.Monitor", -1,
                         OptionMetaData{"monitor", "", true, "MONITOR",
                                        "Set the MONITOR to use, -1 indicates "
                                        "the primary monitor should be used "
                                        "which is the default behaviour"});

  options->setDefaultInt("Render.MSAA", 0);
  options->setDefaultBool("Render.VSync", true);

  // OpenGL
  options->setDefaultInt("Render.GL.MajorVersion", 4);
  options->setDefaultInt("Render.GL.MinorVersion", 5);
}

RenderSystem::~RenderSystem() {}

void RenderSystem::renderOneFrame(const RenderCommand& command)
{
  // TODO
}

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
