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

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/RenderSystem.h"

namespace sequoia {

SEQUOIA_DECLARE_SINGLETON(render::RenderSystem);

namespace render {

RenderSystem::RenderSystem(RenderSystem::RenderSystemKind renderSystemKind)
    : renderSystemKind_(renderSystemKind) {

  try {
    switch(renderSystemKind_) {
    case RK_OpenGL:
      renderSystem_ = std::make_unique<GLRenderSystem>();
    }
  } catch(RenderSystemInitException& e) {
    ErrorHandler::getSingleton().fatal(e.what());
  }
}

int RenderSystem::createWindow(int width, int height, const std::string& title) {
  return renderSystem_->createWindow(width, height, title);
}

RenderWindow* RenderSystem::getWindow(int windowID) { return renderSystem_->getWindow(windowID); }

} // namespace render

} // namespace sequoia
