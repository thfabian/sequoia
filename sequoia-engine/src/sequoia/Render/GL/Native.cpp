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

#include "sequoia/Render/GL/Native.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/NativeGLFW3.h"

namespace sequoia {

namespace render {

std::shared_ptr<NativeGLContext> NativeGLContext::create(NativeWindowSystemKind kind) {
  switch(kind) {
  case NativeWindowSystemKind::NK_GLFW3:
    return std::make_shared<glfw3NativeGLContext>();
  default:
    sequoia_unreachable("invalid window-system");
  }
}

std::shared_ptr<NativeWindow>
NativeWindow::create(const std::shared_ptr<NativeGLContext>& context) {
  switch(context->getKind()) {
  case NativeWindowSystemKind::NK_GLFW3:
    return std::make_shared<glfw3NativeWindow>(context);
  default:
    sequoia_unreachable("invalid window-system");
  }
}

std::shared_ptr<NativeInputSystem>
NativeInputSystem::create(const std::shared_ptr<NativeWindow>& window, bool centerCursor) {
  switch(window->getContext()->getKind()) {
  case NativeWindowSystemKind::NK_GLFW3:
    return std::make_shared<glfw3NativeInputSystem>(window, centerCursor);
  default:
    sequoia_unreachable("invalid window-system");
  }
}

} // namespace render

} // namespace sequoia
