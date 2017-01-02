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
#include <OGRE/OgreMeshManager.h>
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
  vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

  // Alter the camera aspect ratio to match the viewport
  camera_->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

  camera_->setPosition(Ogre::Vector3(0, 0, 10));
  camera_->setNearClipDistance(1.5f);
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
  sceneManager_->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  camera_->setPosition(300, 300, -300);

  Ogre::Light* spotLight = sceneManager_->createLight("SpotLight");
  spotLight->setDiffuseColour(0, 0, 1.0);
  spotLight->setSpecularColour(0, 0, 1.0);
  spotLight->setDirection(-1, -1, 0);
  spotLight->setPosition(Ogre::Vector3(200, 200, 0));
  spotLight->setType(Ogre::Light::LT_SPOTLIGHT);

  Ogre::Light* directionalLight = sceneManager_->createLight("DirectionalLight");
  directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
  directionalLight->setDiffuseColour(Ogre::ColourValue(.4, 0, 0));
  directionalLight->setSpecularColour(Ogre::ColourValue(.4, 0, 0));
  directionalLight->setDirection(Ogre::Vector3(0, -1, 1));

  Ogre::Light* pointLight = sceneManager_->createLight("PointLight");
  pointLight->setType(Ogre::Light::LT_POINT);
  pointLight->setDiffuseColour(.3, .3, .3);
  pointLight->setSpecularColour(.3, .3, .3);
  pointLight->setPosition(Ogre::Vector3(0, 150, 250));

  //
  // Ninja
  //
  Ogre::Entity* ninjaEntity = sceneManager_->createEntity("ninja.mesh");
  ninjaEntity->setCastShadows(true);
  Ogre::SceneNode* ninjaSceneNode = sceneManager_->getRootSceneNode()->createChildSceneNode(); 
  ninjaSceneNode->attachObject(ninjaEntity);

  //
  // Camera orbits ninja
  //
  camera_->lookAt(Ogre::Vector3::ZERO);
  cameraController_->setTarget(ninjaSceneNode);
  cameraController_->setStyle(CameraController::Orbit);

  //
  // Ground plane
  //
  Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
  Ogre::MeshManager::getSingleton().createPlane(
    "ground",
    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    plane, 
    1500, 1500, 20, 20, 
    true, 
    1, 5, 5, 
    Ogre::Vector3::UNIT_Z);

  Ogre::Entity* groundEntity = sceneManager_->createEntity("ground");
  sceneManager_->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
  groundEntity->setCastShadows(false);
  groundEntity->setMaterialName("Examples/Rockwall");
}

} // namespace game

} // namespace sequoia
