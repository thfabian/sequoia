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

namespace sequoia {

namespace game {

CameraController::CameraController(Ogre::Camera* camera) {
  InputManager::getSingleton().addMouseListener(this, "CameraController");
}

bool CameraController::mouseMoved(const OIS::MouseEvent& e) { return true; }

bool CameraController::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
  return true;
}

bool CameraController::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
  return true;
}

} // namespace game

} // namespace sequoia
