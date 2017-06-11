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
      rightKey_(rightKey), upKey_(upKey), downKey_(downKey), topSpeed_(5.0f), velocity_(0),
      goingForward_(false), goingBack_(false), goingLeft_(false), goingRight_(false),
      goingUp_(false), goingDown_(false) {}

CameraControllerFree::CameraControllerFree(const CameraControllerFree& other) : Base(other) {}

CameraControllerFree::~CameraControllerFree() {}

void CameraControllerFree::setPosition(const glm::vec3& position) {
  Base::setPosition(position);
  if(hasCamera()) {
    math::vec3 centerOffsetToEye = getCamera()->getEye() - getCamera()->getCenter();
    getCamera()->setEye(getPosition());
    getCamera()->setEye(getPosition() + centerOffsetToEye);
  }
}

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
  math::mat3 axes = getLocalAxes();

  // Build our acceleration vector based on keyboard input composite
  math::vec3 accel(0);

  if(goingForward_)
    accel += axes[2];
  if(goingBack_)
    accel -= axes[2];
  if(goingRight_)
    accel += axes[0];
  if(goingLeft_)
    accel -= axes[0];
  if(goingUp_)
    accel += axes[1];
  if(goingDown_)
    accel -= axes[1];

  const float topSpeed = fastMove_ ? topSpeed_ * 20 : topSpeed_;

  if(math::length2(accel) != 0) {
    // If accelerating, try to reach top speed in a certain time
    accel = math::normalize(accel);
    velocity_ += accel * topSpeed * event.TimeStep * 1.0f;
  } else
    // If not accelerating, try to stop in a certain time
    velocity_ -= velocity_ * event.TimeStep * 1.0f;

  const float tooSmall = std::numeric_limits<float>::epsilon();

  // Keep camera velocity below top speed and above epsilon
  const float vel2 = math::length2(velocity_);
  if(vel2 > topSpeed * topSpeed) {
    velocity_ = math::normalize(velocity_);
    velocity_ *= topSpeed;
  } else if(vel2 < tooSmall * tooSmall)
    velocity_ = math::vec3(0);

  if(velocity_ != math::vec3(0))
    move(velocity_ * event.TimeStep);
}

void CameraControllerFree::manualStop() {
  goingForward_ = false;
  goingBack_ = false;
  goingLeft_ = false;
  goingRight_ = false;
  goingUp_ = false;
  goingDown_ = false;
  velocity_ = math::vec3(0);
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
