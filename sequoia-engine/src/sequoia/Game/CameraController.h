//===-- sequoia/Game/CameraController.h ---------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_GAME_CAMAERACONTROLLER_H
#define SEQUOIA_GAME_CAMAERACONTROLLER_H

#include "sequoia/Game/InputManager.h"
#include <OGRE/OgrePrerequisites.h>

namespace sequoia {

namespace game {

class CameraController : public OIS::MouseListener {
public:
  CameraController(Ogre::Camera* camera);

  bool mouseMoved(const OIS::MouseEvent& e);
  bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
private:
  Ogre::Camera* camera_; ///< Reference to the camera
};

} // namespace game

} // namespace sequoia

#endif
