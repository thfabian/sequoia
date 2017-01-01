//===-- sequoia/Game/CameraController.h ---------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Game/CameraController.h"
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <limits>

namespace sequoia {

namespace game {

// -------------------------------------------------------------------------------------------------
CameraController::CameraController(Ogre::Camera* camera)
    : camera_(camera), target_(nullptr), orbiting_(false), zooming_(false), topSpeed_(150),
      velocity_(Ogre::Vector3::ZERO), goingForward_(false), goingBack_(false), goingLeft_(false),
      goingRight_(false), goingUp_(false), goingDown_(false), fastMove_(false) {

  setStyle(CameraStyle::Freelock);
  InputManager::getSingleton().addKeyListener(this, "CameraController");
}

// -------------------------------------------------------------------------------------------------
void CameraController::addAsMouseListener() {
  InputManager::getSingleton().addMouseListener(this, "CameraController");
}

// -------------------------------------------------------------------------------------------------
void CameraController::removeAsMouseListener() {
  InputManager::getSingleton().removeMouseListener("CameraController");
}

// -------------------------------------------------------------------------------------------------
void CameraController::addAsKeyListener() {
  InputManager::getSingleton().addKeyListener(this, "CameraController");
}

// -------------------------------------------------------------------------------------------------
void CameraController::removeAsKeyListener() {
  InputManager::getSingleton().removeKeyListener("CameraController");
}

// -------------------------------------------------------------------------------------------------
void CameraController::addAsFrameListener(std::shared_ptr<Ogre::Root>& root) {
  root->addFrameListener(this);
}

// -------------------------------------------------------------------------------------------------
void CameraController::removeAsFrameListener(std::shared_ptr<Ogre::Root>& root) {
  root->removeFrameListener(this);
}

// -------------------------------------------------------------------------------------------------
void CameraController::setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist) {
  camera_->setPosition(target_->_getDerivedPosition());
  camera_->setOrientation(target_->_getDerivedOrientation());
  camera_->yaw(yaw);
  camera_->pitch(-pitch);
  camera_->moveRelative(Ogre::Vector3(0, 0, dist));
}

// -------------------------------------------------------------------------------------------------
void CameraController::manualStop() {
  if(style_ == CameraStyle::Freelock) {
    goingForward_ = false;
    goingBack_ = false;
    goingLeft_ = false;
    goingRight_ = false;
    goingUp_ = false;
    goingDown_ = false;
    velocity_ = Ogre::Vector3::ZERO;
  }
}

// -------------------------------------------------------------------------------------------------
void CameraController::setCamera(Ogre::Camera* camera) { camera_ = camera; }

// -------------------------------------------------------------------------------------------------
Ogre::Camera* CameraController::camera() const noexcept { return camera_; }

// -------------------------------------------------------------------------------------------------
void CameraController::setTarget(Ogre::SceneNode* target) {
  if(target != target_) {
    target_ = target;
    if(target) {
      setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
      camera_->setAutoTracking(true, target_);
    } else {
      camera_->setAutoTracking(false);
    }
  }
}

// -------------------------------------------------------------------------------------------------
Ogre::SceneNode* CameraController::target() const noexcept { return target_; }

// -------------------------------------------------------------------------------------------------
void CameraController::setStyle(CameraStyle style) {
  if(style_ != CameraStyle::Orbit && style == CameraStyle::Orbit) {
    setTarget(target_ ? target_ : camera_->getSceneManager()->getRootSceneNode());
    camera_->setFixedYawAxis(true);
    manualStop();
    setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
  } else if(style_ != CameraStyle::Freelock && style == CameraStyle::Freelock) {
    camera_->setAutoTracking(false);
    camera_->setFixedYawAxis(true);
  } else if(style_ != CameraStyle::Manual && style == CameraStyle::Manual) {
    camera_->setAutoTracking(false);
    manualStop();
  }
  style_ = style;
}

// -------------------------------------------------------------------------------------------------
CameraController::CameraStyle CameraController::style() const noexcept { return style_; }

// -------------------------------------------------------------------------------------------------
void CameraController::setTopSpeed(Ogre::Real topSpeed) { topSpeed_ = topSpeed; }

// -------------------------------------------------------------------------------------------------
Ogre::Real CameraController::topSpeed() const noexcept { return topSpeed_; }

// -------------------------------------------------------------------------------------------------
bool CameraController::mouseMoved(const OIS::MouseEvent& e) {
  if(style_ == CameraStyle::Orbit) {
    Ogre::Real dist = (camera_->getPosition() - target_->_getDerivedPosition()).length();

    // Yaw around the target, and pitch locally
    if(orbiting_) {
      camera_->setPosition(target_->_getDerivedPosition());

      camera_->yaw(Ogre::Degree(-e.state.X.rel * 0.25f));
      camera_->pitch(Ogre::Degree(-e.state.Y.rel * 0.25f));

      camera_->moveRelative(Ogre::Vector3(0, 0, dist));
    }
    // Move the camera toward or away from the target
    else if(zooming_) {
      // The further the camera is, the faster it moves
      camera_->moveRelative(Ogre::Vector3(0, 0, e.state.Y.rel * 0.004f * dist));
    }
    // Move the camera toward or away from the target
    else if(e.state.Z.rel != 0) {
      // The further the camera is, the faster it moves
      camera_->moveRelative(Ogre::Vector3(0, 0, -e.state.Z.rel * 0.0008f * dist));
    }
  } else if(style_ == CameraStyle::Freelock) {
    camera_->yaw(Ogre::Degree(-e.state.X.rel * 0.15f));
    camera_->pitch(Ogre::Degree(-e.state.Y.rel * 0.15f));
  }

  return true;
}

// -------------------------------------------------------------------------------------------------
bool CameraController::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
  if(style_ == CameraStyle::Orbit) {
    if(id == OIS::MB_Left)
      orbiting_ = true;
    else if(id == OIS::MB_Right)
      zooming_ = true;
  }
  return true;
}

// -------------------------------------------------------------------------------------------------
bool CameraController::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
  if(style_ == CameraStyle::Orbit) {
    if(id == OIS::MB_Left)
      orbiting_ = false;
    else if(id == OIS::MB_Right)
      zooming_ = false;
  }
  return true;
}

// -------------------------------------------------------------------------------------------------
bool CameraController::keyPressed(const OIS::KeyEvent& e) {
  if(style_ == CameraStyle::Freelock) {
    if(e.key == OIS::KC_W || e.key == OIS::KC_UP)
      goingForward_ = true;
    else if(e.key == OIS::KC_S || e.key == OIS::KC_DOWN)
      goingBack_ = true;
    else if(e.key == OIS::KC_A || e.key == OIS::KC_LEFT)
      goingLeft_ = true;
    else if(e.key == OIS::KC_D || e.key == OIS::KC_RIGHT)
      goingRight_ = true;
    else if(e.key == OIS::KC_PGUP)
      goingUp_ = true;
    else if(e.key == OIS::KC_PGDOWN)
      goingDown_ = true;
    else if(e.key == OIS::KC_LSHIFT)
      fastMove_ = true;
  }
  return true;
}

// -------------------------------------------------------------------------------------------------
bool CameraController::keyReleased(const OIS::KeyEvent& e) {
  if(style_ == CameraStyle::Freelock) {
    if(e.key == OIS::KC_W || e.key == OIS::KC_UP)
      goingForward_ = false;
    else if(e.key == OIS::KC_S || e.key == OIS::KC_DOWN)
      goingBack_ = false;
    else if(e.key == OIS::KC_A || e.key == OIS::KC_LEFT)
      goingLeft_ = false;
    else if(e.key == OIS::KC_D || e.key == OIS::KC_RIGHT)
      goingRight_ = false;
    else if(e.key == OIS::KC_PGUP)
      goingUp_ = false;
    else if(e.key == OIS::KC_PGDOWN)
      goingDown_ = false;
    else if(e.key == OIS::KC_LSHIFT)
      fastMove_ = false;
  }
  return true;
}

// -------------------------------------------------------------------------------------------------
bool CameraController::frameRenderingQueued(const Ogre::FrameEvent& evt) {
  if(style_ == CameraStyle::Freelock) {
    // Build our acceleration vector based on keyboard input composite
    Ogre::Vector3 accel = Ogre::Vector3::ZERO;
    if(goingForward_)
      accel += camera_->getDirection();
    if(goingBack_)
      accel -= camera_->getDirection();
    if(goingRight_)
      accel += camera_->getRight();
    if(goingLeft_)
      accel -= camera_->getRight();
    if(goingUp_)
      accel += camera_->getUp();
    if(goingDown_)
      accel -= camera_->getUp();

    // If accelerating, try to reach top speed in a certain time
    Ogre::Real topSpeed = fastMove_ ? topSpeed_ * 20 : topSpeed_;
    if(accel.squaredLength() != 0) {
      accel.normalise();
      velocity_ += accel * topSpeed * evt.timeSinceLastFrame * 10;
    }
    // If not accelerating, try to stop in a certain time
    else
      velocity_ -= velocity_ * evt.timeSinceLastFrame * 10;

    Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

    // Keep camera velocity below top speed and above epsilon
    if(velocity_.squaredLength() > topSpeed * topSpeed) {
      velocity_.normalise();
      velocity_ *= topSpeed;
    } else if(velocity_.squaredLength() < tooSmall * tooSmall)
      velocity_ = Ogre::Vector3::ZERO;

    if(velocity_ != Ogre::Vector3::ZERO)
      camera_->move(velocity_ * evt.timeSinceLastFrame);
  }

  return true;
}

} // namespace game

} // namespace sequoia
