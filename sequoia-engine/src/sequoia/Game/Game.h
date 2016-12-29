//===-- sequoia/Game/Game.h ---------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Main class holding all Ogre related objects.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_GAME_GAME_H
#define SEQUOIA_GAME_GAME_H

#include "sequoia/Game/Export.h"
#include <OGRE/OgreFrameListener.h>
#include <OGRE/OgrePrerequisites.h>
#include <OGRE/OgreSingleton.h>
#include <OGRE/OgreWindowEventUtilities.h>
#include <memory>

namespace Ogre {
class OverlaySystem;
}

namespace sequoia {

namespace game {

/// @brief Main class holding all Ogre related objects and running the main-loop
///
/// @ingroup game
class SEQUOIA_GAME_EXPORT Game : public Ogre::Singleton<Game>,
                                 public Ogre::FrameListener,
                                 public Ogre::WindowEventListener {
public:
  Game();
  ~Game();

  /// @brief Run the main-loop
  void run();

private:
  void setup();

  void setupResources();

  void chooseSceneManager();

  void createCamera();

  void createFrameListener();

  void createScene();

  void destroyScene();

  void createViewports();

  void createResourceListener();

  void loadResources();

  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
  
  
  // -- TMP --
  void setupDummyScene();
  // ---------
  

  /// @brief Callback for resize event
  virtual void windowResized(Ogre::RenderWindow* window);

  /// @brief Callback for window close event
  ///
  /// Unattaches OIS before window shutdown (very important under Linux).
  virtual void windowClosed(Ogre::RenderWindow* window);

private:
  std::shared_ptr<Ogre::Root> root_;
  
  // Window
  Ogre::RenderWindow* renderWindow_;
  Ogre::RenderSystem* renderSystem_;

  // Scene
  Ogre::SceneManager* sceneManager_;
  Ogre::OverlaySystem* overlaySystem_;

  // Camera
  Ogre::Camera* camera_;
};

} // namespace game

} // namespace sequoia

#endif
