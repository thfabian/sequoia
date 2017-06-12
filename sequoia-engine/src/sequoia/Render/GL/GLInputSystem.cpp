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

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLInputSystem.h"

namespace sequoia {

namespace render {

GLInputSystem::GLInputSystem(RenderTarget* target, GLFWwindow* window)
    : target_(target), prevPosX_(0), prevPosY_(0) {
  double xPos, yPos;
  glfwGetCursorPos(window, &xPos, &yPos);
  prevPosX_ = xPos;
  prevPosY_ = yPos;
}

void GLInputSystem::keyCallback(int key, int action, int mods) noexcept {
  KeyboardEvent event{target_, (KeyboardKey)key, (KeyAction)action, mods};
  for(KeyboardListener* listener : getListeners<KeyboardListener>())
    listener->keyboardEvent(event);
}

void GLInputSystem::mouseButtonCallback(int button, int action, int mods) noexcept {
  MouseButtonEvent event{target_, (MouseButton)button, (KeyAction)action, mods};
  for(MouseListener* listener : getListeners<MouseListener>())
    listener->mouseButtonEvent(event);
}

void GLInputSystem::mousePositionCallback(int xPos, int yPos) noexcept {
  MousePositionEvent event{target_, xPos, yPos, xPos - prevPosX_, yPos - prevPosY_};
  for(MouseListener* listener : getListeners<MouseListener>())
    listener->mousePositionEvent(event);
  prevPosX_ = xPos;
  prevPosY_ = yPos;
}

} // namespace render

} // namespace sequoia
