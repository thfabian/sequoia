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
#include "sequoia/Render/GL/GLRenderWindow.h"

namespace sequoia {

namespace render {

GLInputSystem::GLInputSystem(GLRenderWindow* window, bool centerCursor)
    : window_(window), prevPosX_(0), prevPosY_(0) {

  if(centerCursor)
    this->centerCursor();

  double xPos, yPos;
  glfwGetCursorPos(window_->getGLFWwindow(), &xPos, &yPos);
  prevPosX_ = xPos;
  prevPosY_ = yPos;
}

void GLInputSystem::keyCallback(int key, int action, int mods) noexcept {
  KeyboardEvent event{window_, (KeyboardKey)key, (KeyAction)action, mods};
  for(KeyboardListener* listener : getListeners<KeyboardListener>())
    listener->keyboardEvent(event);
}

void GLInputSystem::mouseButtonCallback(int button, int action, int mods) noexcept {
  MouseButtonEvent event{window_, (MouseButton)button, (KeyAction)action, mods};
  for(MouseListener* listener : getListeners<MouseListener>())
    listener->mouseButtonEvent(event);
}

void GLInputSystem::mousePositionCallback(int xPos, int yPos) noexcept {
  MousePositionEvent event{window_, xPos, yPos, xPos - prevPosX_, yPos - prevPosY_};
  for(MouseListener* listener : getListeners<MouseListener>())
    listener->mousePositionEvent(event);
  prevPosX_ = xPos;
  prevPosY_ = yPos;
}

void GLInputSystem::centerCursor() {
  int xPos = window_->getWidth() / 2;
  int yPos = window_->getHeight() / 2;
  setCursorPosition(xPos, yPos);
  prevPosX_ = xPos;
  prevPosY_ = yPos;
}

void GLInputSystem::setCursorPosition(int xpos, int ypos) {
  glfwSetCursorPos(window_->getGLFWwindow(), xpos, ypos);
}

} // namespace render

} // namespace sequoia
