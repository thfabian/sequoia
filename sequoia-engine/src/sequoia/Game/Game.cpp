//===-- sequoia/Game/Game.cpp -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Core/SingletonManager.h"
#include "sequoia/Core/SmallVector.h"
#include "sequoia/Core/StringRef.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/InputManager.h"
#include "sequoia/Game/RenderSystemFactory.h"
#include "sequoia/Game/WindowFactory.h"
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreTimer.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>

template <>
sequoia::game::Game* Ogre::Singleton<sequoia::game::Game>::msSingleton = nullptr;

namespace sequoia {

namespace game {

Game::Game() {}

Game::~Game() {}

// -------------------------------------------------------------------------------------------------
void Game::run() {
  //
  // Setup
  //
  auto timer = Ogre::Timer();
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Start Setup ***");
  setup();
  Ogre::LogManager::getSingletonPtr()->logMessage(
      core::format("*** Setup done (took %lu ms) ***", timer.getMilliseconds()));

  //
  // Start rendering
  //
  root_->clearEventTimes();
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Start Rendering ***");
  while(!renderWindow_->isClosed()) {
    // Update Screen
    renderWindow_->update(false);
    renderWindow_->swapBuffers();
    root_->renderOneFrame();

    // Update render windows
    Ogre::WindowEventUtilities::messagePump();
  }
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Stop Rendering ***");

  //
  // Tear-down
  //
  tearDown();
}

// -------------------------------------------------------------------------------------------------
void Game::setup() {
  auto& config = GlobalConfiguration::getSingleton();

  // Create root object
  root_ = std::make_shared<Ogre::Root>(config.getPath("InternalSettings.PluginPath").string(),
                                       config.getPath("InternalSettings.ConfigPath").string(), "");

  // Setup resources
  setupResources();

  // Load plugins
  SmallVector<StringRef, 5> plugins;
  StringRef(SEQUOIA_OGRE_PLUGINS).split(plugins, ";");
  for(const auto& pluginStr : plugins) {
    auto plugin = pluginStr.str();
    root_->loadPlugin(plugin);
  }

  // Register render subsystem
  renderSystem_ =
      RenderSystemFactory::create(root_, config.getBoolean("CommandLine.ShowRenderDialog", false),
                                  config.getString("CommandLine.RenderSystem", "OpenGL"));

  // Initialize root and register myself as FrameListener
  root_->initialise(false);
  root_->addFrameListener(this);

  // Create RenderWindow
  renderWindow_ = WindowFactory::create(root_);
  renderWindow_->setActive(true);
  renderWindow_->setAutoUpdated(false);

  // Initialize Input/Output System (OIS)
  auto* inputSystem = SingletonManager::getSingleton().allocateSingleton<InputManager>();
  inputSystem->init(renderWindow_);

  // Choose SceneManger and create Camera
  chooseSceneManager();
  createCamera();
  createViewports();

  // Set default mipmap level
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

  // Create any resource listeners (for loading screens)
  createResourceListener();

  // Load resources
  loadResources();

  // Create the scene
  createScene();
}

// -------------------------------------------------------------------------------------------------
void Game::tearDown() {
  InputManager::getSingleton().finalize();
  destroyScene();
}

// -------------------------------------------------------------------------------------------------
void Game::setupResources() {
  auto& config = GlobalConfiguration::getSingleton();

  // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load(config.getPath("InternalSettings.ConfigPath").string());

  // Go through all sections & settings in the file
  Ogre::ConfigFile::SectionIterator sectionIt = cf.getSectionIterator();

  Ogre::String sectionName, typeName, archName;
  while(sectionIt.hasMoreElements()) {
    sectionName = sectionIt.peekNextKey();
    Ogre::ConfigFile::SettingsMultiMap* settings = sectionIt.getNext();
    for(auto i = settings->begin(); i != settings->end(); ++i) {
      typeName = i->first;
      archName = i->second;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
      // OS X does not set the working directory relative to the app. In order to make things
      // portable on OS X we need to provide the loading with it's own bundle path location.
      if(!Ogre::StringUtil::startsWith(archName, "/", false))
        archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName,
                                                                     sectionName);
    }
  }
}

// -------------------------------------------------------------------------------------------------
void Game::createResourceListener() {}

// -------------------------------------------------------------------------------------------------
void Game::loadResources() {
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

// -------------------------------------------------------------------------------------------------
void Game::chooseSceneManager() {
  Ogre::LogManager::getSingletonPtr()->logMessage("Initializing SceneManager ...");

  // Get the SceneManager
  sceneManager_ = root_->createSceneManager(Ogre::ST_GENERIC, "SceneManager");

  // Initialize the OverlaySystem
  overlaySystem_ = new Ogre::OverlaySystem();
  sceneManager_->addRenderQueueListener(overlaySystem_);

  Ogre::LogManager::getSingletonPtr()->logMessage("Successfully initialized SceneManager");
}

// -------------------------------------------------------------------------------------------------
void Game::createCamera() {
  Ogre::LogManager::getSingletonPtr()->logMessage("Creating Camera ...");

  // Create the camera
  camera_ = sceneManager_->createCamera("Camera");

  // Insert camera as a scene node
  Ogre::SceneNode* cameraNode = sceneManager_->getRootSceneNode()->createChildSceneNode("Camera");
  cameraNode->attachObject(camera_);

  // Setup camera controller
  cameraController_ = std::make_unique<CameraController>(camera_);
  cameraController_->addAsMouseListener();
  cameraController_->addAsKeyListener();
  cameraController_->addAsFrameListener(root_);

  Ogre::LogManager::getSingletonPtr()->logMessage("Successfully initialized Camera");
}

// -------------------------------------------------------------------------------------------------
void Game::createScene() { setupDummyScene(); }

// -------------------------------------------------------------------------------------------------
void Game::destroyScene() {}

// -------------------------------------------------------------------------------------------------
void Game::createViewports() {
  // Create one viewport, entire window
  Ogre::Viewport* vp = renderWindow_->addViewport(camera_);
  vp->setBackgroundColour(Ogre::ColourValue(1, 1, 1));

  // Alter the camera aspect ratio to match the viewport
  camera_->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

  camera_->setPosition(Ogre::Vector3(0, 0, 10));
  camera_->setNearClipDistance(1.5f);
  camera_->setFarClipDistance(3000.0f);
}

// -------------------------------------------------------------------------------------------------
bool Game::frameRenderingQueued(const Ogre::FrameEvent& e) {
  if(renderWindow_->isClosed())
    return false;

  // Capture Keyboard/Mouse
  InputManager::getSingleton().capture();

  return true;
}

// -------------------------------------------------------------------------------------------------
void Game::setupDummyScene() {
  sceneManager_->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  camera_->setPosition(0, 47, 222);

  Ogre::Light* light = sceneManager_->createLight("MainLight");
  light->setPosition(20.0, 80.0, 50.0);

  Ogre::Entity* ogreEntity = sceneManager_->createEntity("ogrehead.mesh");

  Ogre::SceneNode* ogreNode = sceneManager_->getRootSceneNode()->createChildSceneNode();
  ogreNode->attachObject(ogreEntity);

  Ogre::Entity* ogreEntity2 = sceneManager_->createEntity("ogrehead.mesh");

  Ogre::SceneNode* ogreNode2 =
      sceneManager_->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(84, 48, 0));
  ogreNode2->attachObject(ogreEntity2);

  Ogre::Entity* ogreEntity3 = sceneManager_->createEntity("ogrehead.mesh");

  Ogre::SceneNode* ogreNode3 = sceneManager_->getRootSceneNode()->createChildSceneNode();
  ogreNode3->setPosition(Ogre::Vector3(0, 104, 0));
  ogreNode3->setScale(2, 1.2, 1);
  ogreNode3->attachObject(ogreEntity3);

  Ogre::Entity* ogreEntity4 = sceneManager_->createEntity("ogrehead.mesh");

  Ogre::SceneNode* ogreNode4 = sceneManager_->getRootSceneNode()->createChildSceneNode();
  ogreNode4->setPosition(-84, 48, 0);
  ogreNode4->roll(Ogre::Degree(-90));
  ogreNode4->attachObject(ogreEntity4);
}

} // namespace game

} // namespace sequoia
