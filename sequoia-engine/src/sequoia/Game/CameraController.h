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

#include <OGRE/OgrePrerequisites.h>

namespace sequoia {

namespace game {

class CameraController {
public:
  CameraController(Ogre::Camera* camera);

private:
  Ogre::Camera* camera_; ///< Reference to the camera

};

} // namespace game

} // namespace sequoia

#endif