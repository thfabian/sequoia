//===-- sequoia/Game/Game.h ---------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
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
                                 public Ogre::FrameListener {
public:
  Game();
  ~Game();

  /// @brief Run the main-loop
  void run();

private:
  void setup();

  void tearDown();

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
