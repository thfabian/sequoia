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
#include <OGRE/OgreFrameListener.h>
#include <OGRE/OgreMath.h>
#include <OGRE/OgreVector3.h>

namespace sequoia {

namespace game {

/// @brief Utility class for controlling the Camera in the scene
///
/// @ingroup game
class CameraController : public OIS::MouseListener,
                         public OIS::KeyListener,
                         public Ogre::FrameListener {
public:
  /// @brief Style of camera movements
  enum CameraStyle { Freelock, Orbit, Manual };

  /// @brief Initialize with an already existing Camera
  CameraController(Ogre::Camera* camera);

  /// @brief Sets the spatial offset from the target. Only applies for orbit style
  void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist);

  /// @brief Manually stops the camera when in free-look mode
  void manualStop();

  /// @brief Add/Remove as Listener
  /// @{
  void addAsMouseListener();
  void removeAsMouseListener();

  void addAsKeyListener();
  void removeAsKeyListener();

  void addAsFrameListener(std::shared_ptr<Ogre::Root>& root);
  void removeAsFrameListener(std::shared_ptr<Ogre::Root>& root);
  /// @}

  /// @brief Setter/Getter
  /// @{
  void setCamera(Ogre::Camera* camera);
  Ogre::Camera* camera() const noexcept;

  void setTarget(Ogre::SceneNode* target);
  Ogre::SceneNode* target() const noexcept;

  void setStyle(CameraStyle style);
  CameraStyle style() const noexcept;

  void setTopSpeed(Ogre::Real topSpeed);
  Ogre::Real topSpeed() const noexcept;
  /// @}

  /// @brief MouseListener implementation
  /// @{
  virtual bool mouseMoved(const OIS::MouseEvent& e) override;
  virtual bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) override;
  virtual bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) override;
  /// @}

  /// @brief KeyListener implementation
  /// @{
  virtual bool keyPressed(const OIS::KeyEvent& e) override;
  virtual bool keyReleased(const OIS::KeyEvent& e) override;
  /// @}

  /// @brief FrameListener implementation
  /// @{
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& e) override;
  /// @}

private:
  Ogre::Camera* camera_;
  Ogre::SceneNode* target_;
  CameraStyle style_;

  bool orbiting_;
  bool zooming_;
  Ogre::Real topSpeed_;
  Ogre::Vector3 velocity_;
  bool goingForward_;
  bool goingBack_;
  bool goingLeft_;
  bool goingRight_;
  bool goingUp_;
  bool goingDown_;
  bool fastMove_;
};

} // namespace game

} // namespace sequoia

#endif
