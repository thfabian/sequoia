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

#include "sequoia/Core/Format.h"
#include "sequoia/Game/CameraControllerFree.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Math/CoordinateSystem.h"

#include <iostream>

namespace sequoia {

namespace game {

CameraControllerFree::CameraControllerFree(const std::string& name, SceneNodeKind kind,
                                           const std::shared_ptr<Keymap>& forwardKey,
                                           const std::shared_ptr<Keymap>& backwardKey,
                                           const std::shared_ptr<Keymap>& leftKey,
                                           const std::shared_ptr<Keymap>& rightKey,
                                           const std::shared_ptr<Keymap>& upKey,
                                           const std::shared_ptr<Keymap>& downKey)
    : Base(name, kind), forwardKey_(forwardKey), backwardKey_(backwardKey), leftKey_(leftKey),
      rightKey_(rightKey), upKey_(upKey), downKey_(downKey), speed_(5.0f), goingForward_(false),
      goingBack_(false), goingLeft_(false), goingRight_(false), goingUp_(false), goingDown_(false) {
}

CameraControllerFree::CameraControllerFree(const CameraControllerFree& other) : Base(other) {}

CameraControllerFree::~CameraControllerFree() {}

void CameraControllerFree::setCamera(const std::shared_ptr<render::Camera>& camera) {
  Base::setCamera(camera);
  Game::getSingleton().addListener(static_cast<MouseListener*>(this));
  Game::getSingleton().addListener(static_cast<KeyListener*>(this));
}

void CameraControllerFree::removeCamera() {
  Base::removeCamera();
  Game::getSingleton().removeListener(static_cast<MouseListener*>(this));
  Game::getSingleton().removeListener(static_cast<KeyListener*>(this));
}

void CameraControllerFree::update(const UpdateEvent& event) {
  if(!hasCamera())
    return;
  
  if(!updateNeeded_ && !goingForward_ && !goingBack_ && !goingRight_ && !goingLeft_ && !goingUp_ &&
     !goingDown_)
    return;

  math::mat3 axes = getLocalAxes();
  math::vec3 dir(0);

//  pitch(math::Radian::fromDegree(3.0f));  
  //std::cout << axes[2] << std::endl;

  if(goingForward_)
    dir += axes[2];
  if(goingBack_)
    dir -= axes[2];
  if(goingRight_)
    dir -= axes[0];
  if(goingLeft_)
    dir += axes[0];
  if(goingUp_)
    dir += axes[1];
  if(goingDown_)
    dir -= axes[1];

  if(dir != math::vec3(0)) {
    dir = math::normalize(dir);
    translate(speed_ * event.TimeStep * dir);
  }

  // Update camera position
  Base::update(event);
}

std::shared_ptr<SceneNode> CameraControllerFree::clone() {
  return SceneGraph::create<CameraControllerFree>(*this);
}

void CameraControllerFree::keyboardEvent(const render::KeyboardEvent& event) {
  if(!hasCamera())
    return;

  const bool enable = event.pressed() || event.repeated();

  if(forwardKey_->handle(event))
    goingForward_ = enable;
  else if(backwardKey_->handle(event))
    goingBack_ = enable;
  else if(rightKey_->handle(event))
    goingRight_ = enable;
  else if(leftKey_->handle(event))
    goingLeft_ = enable;
  else if(upKey_->handle(event))
    goingUp_ = enable;
  else if(downKey_->handle(event))
    goingDown_ = enable;
}

void CameraControllerFree::mouseButtonEvent(const render::MouseButtonEvent& event) {
  if(!hasCamera())
    return;
}

void CameraControllerFree::mousePositionEvent(const render::MousePositionEvent& event) {
  if(!hasCamera())
    return;
}

std::pair<std::string, std::string> CameraControllerFree::toStringImpl() const {
  return std::make_pair("CameraControllerFree",
                        core::format("%s"
                                     "forwardKey = %s,\n"
                                     "backwardKey = %s,\n"
                                     "leftKey = %s,\n"
                                     "rightKey = %s,\n"
                                     "upKey = %s,\n"
                                     "downKey = %s,\n",
                                     Base::toStringImpl().second, forwardKey_->toString(),
                                     backwardKey_->toString(), leftKey_->toString(),
                                     rightKey_->toString(), upKey_->toString(),
                                     downKey_->toString()));
}

} // namespace game

} // namespace sequoia
