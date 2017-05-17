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

#include "sequoia/Render/GL/GLInputSystem.h"

namespace sequoia {

namespace render {

GLInputSystem::GLInputSystem(RenderTarget* target) : target_(target) {}

void GLInputSystem::keyCallback(int key, int action, int mods) noexcept {
  KeyboardEvent event{target_, (KeyboardKey)key, (KeyAction)action, mods};
  for(KeyboardListener* listener : getListeners<KeyboardListener>())
    listener->keyboardEvent(event);
}

void GLInputSystem::mouseCallback(int button, int action, int mods) noexcept {
  MouseEvent event{target_, (MouseButton)button, (KeyAction)action, mods};
  for(MouseListener* listener : getListeners<MouseListener>())
    listener->mouseEvent(event);
}

} // namespace render

} // namespace sequoia
