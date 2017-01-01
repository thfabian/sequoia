//===-- sequoia/Game/InputManager.h -------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_GAME_INPUTMANAGER_H
#define SEQUOIA_GAME_INPUTMANAGER_H

#include <OGRE/OgrePrerequisites.h>
#include <OGRE/OgreSingleton.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#include <OIS/OISPrereqs.h>
#include <memory>
#include <string>

namespace sequoia {

namespace game {

/// @brief Initialize the Object-oriented Input System (OIS)
///
/// @ingroup game
class InputManager : public Ogre::Singleton<InputManager> {
public:
  InputManager();
  ~InputManager();

  /// @brief Initialize OIS
  ///
  /// @throw IOException  Initalization failed
  void init(Ogre::RenderWindow* window);

  /// @brief Clean-up OIS
  void finalize();

  /// @brief Capture/update each device every frame
  void capture();

  /// @brief Register to listen to any changes of the Keyboard
  void addKeyListener(OIS::KeyListener* keyListener, const std::string& name);
  void removeKeyListener(const std::string& name);

  /// @brief Register to listen to any changes of the Mouse
  void addMouseListener(OIS::MouseListener* mouseListener, const std::string& name);
  void removeMouseListener(const std::string& name);

  /// @brief Set mouse region (if the Window resizes, the mouse region must be adjusted accordingly)
  void setWindowExtents(int width, int height);

  /// @brief Check if OIS is properly initialized
  bool isInitialized() const noexcept;

private:
  class InputManagerImpl;
  std::unique_ptr<InputManagerImpl> impl_;
};

} // namespace game

} // namespace sequoia

#endif
