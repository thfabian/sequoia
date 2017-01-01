//===-- sequoia/Game/Game.cpp -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Core/SmallVector.h"
#include "sequoia/Core/StringRef.h"
#include "sequoia/Core/SingletonManager.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/RenderSystemFactory.h"
#include "sequoia/Game/WindowFactory.h"
#include "sequoia/Game/InputManager.h"
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreManualObject.h>
#include <OGRE/OgreMesh.h>

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
  setup();

  setupDummyScene();

  //
  // Start rendering
  //
  root_->clearEventTimes();
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Start rendering ***");
  while(!renderWindow_->isClosed()) {
    // Capture Keyboard/Mouse
    InputManager::getSingleton().capture();

    // Update Screen
    renderWindow_->update(false);
    renderWindow_->swapBuffers();
    root_->renderOneFrame();

    // Update render windows 
    Ogre::WindowEventUtilities::messagePump();
  }
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Stop rendering ***");

  //
  // Tear-down
  //
  tearDown();
}

// -------------------------------------------------------------------------------------------------
void Game::setup() {
  auto& config = GlobalConfiguration::getSingleton();

  //
  // Create root object
  //
  root_ = std::make_shared<Ogre::Root>(config.getPath("InternalSettings.PluginPath").string(),
                                       config.getPath("InternalSettings.ConfigPath").string(), "");

  //
  // Setup resources
  //
  setupResources();

  //
  // Load plugins
  //
  SmallVector<StringRef, 5> plugins;
  StringRef(SEQUOIA_OGRE_PLUGINS).split(plugins, ";");
  for(const auto& pluginStr : plugins) {
    auto plugin = pluginStr.str();
    root_->loadPlugin(plugin);
  }

  //
  // Register render subsystem
  //
  renderSystem_ = RenderSystemFactory::create(
      root_, config.getBoolean("CommandLine.ShowRenderDialog", false),
      config.getString("CommandLine.RenderSystem", "OpenGL"));

  //
  // Initialize root
  //
  root_->initialise(false);

  //
  // Create RenderWindow
  //
  renderWindow_ = WindowFactory::create(root_);
  renderWindow_->setActive(true);
  renderWindow_->setAutoUpdated(false);

  //
  // Initialize Input/Output System (OIS)
  //
  auto* inputSystem = SingletonManager::getSingleton().allocateSingleton<InputManager>();
  inputSystem->init(renderWindow_);

  //
  // Choose SceneManger and create Camera
  //
  chooseSceneManager();
  createCamera();
  createViewports();

  //
  // Set default mipmap level
  //
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

  //
  // Create any resource listeners (for loading screens)
  //
  createResourceListener();

  //
  // Load resources
  //
  loadResources();

  //
  // Create the scene
  //
  createScene();

  //
  // Create any frame listeners
  //
  createFrameListener();
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
  // Get the SceneManager
  sceneManager_ = root_->createSceneManager(Ogre::ST_GENERIC, "MySceneManager");

  // Initialize the OverlaySystem
  overlaySystem_ = new Ogre::OverlaySystem();
  sceneManager_->addRenderQueueListener(overlaySystem_);
}

// -------------------------------------------------------------------------------------------------
void Game::createCamera() {
  // Create the camera
  camera_ = sceneManager_->createCamera("MyCamera");

  // Insert camera as a scene node
  Ogre::SceneNode* cameraNode =
      sceneManager_->getRootSceneNode()->createChildSceneNode("MyCameraSceneNode");
  cameraNode->attachObject(camera_);
  
  // Setup camera controller
  cameraController_ = std::make_unique<CameraController>(camera_);
}

// -------------------------------------------------------------------------------------------------
void Game::createFrameListener() {}

// -------------------------------------------------------------------------------------------------
void Game::createScene() {}

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
bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt) {
  if(renderWindow_->isClosed())
    return false;
  return true;
}

// -------------------------------------------------------------------------------------------------
void Game::setupDummyScene() {
  // A simplistic presentation of some realtime 3D basics.
  // In 3D you need most of the time :
  // a vertex table Vt + an index table Idt + a kind of primitive P + an associated material M.
  // Let's say P is "triangle" (most common), then Idt will contain
  // a list of index taken from Vt, where each group of 3 index means a triangle.
  // so (Idt[0], Idt[1],Idt[2]) is the first triangle, (Idt[3],Idt[4],Idt[5]) is the second, etc...
  //
  // When times come to render this whole thing,
  // it will be multiplied by 2 matrix.
  // 1/ a matrix representing transformation(position/orientation/scale) of the thing relative to
  // the camera.
  // 2/ a perspective matrix, corresponding to the parameters of the viewing camera.
  // Ogre will prepare both matrix for you. But you can also provide them if you want.
  // This calculation can be modified in vertex shaders.
  //
  // Once it has been projected, the graphic card uses the buffers (mainly colour and depth)
  // , vertex attributes and material informations (from M) to draw things.
  // vertex attributes are used to 'light', 'colourize', and 'texture' the meshes.
  // This calculation can be partially modified in pixel shaders.
  // This was a very simplistic presentation of 3D rendering ^^.
  {
    // Here, I create a 3D element, by using the interface of ManualObject.
    // ManualObject is very close to the opengl old simple way to specify geometry.
    // There are other interfaces (Hardwarebuffers), you can check the ogremanual fo them and wiki.
    // For each vertex I will provide positions and attributes (normal, vertex color, texture
    // coordinates...).
    // Then for each primitive (given its type : triangle, line, line strip etc...),
    // I give the corresponding group of vertex index.
    Ogre::ManualObject* lManualObject = NULL;
    {
      // The manualObject creation requires a name.
      Ogre::String lManualObjectName = "CubeWithAxes";
      lManualObject = sceneManager_->createManualObject(lManualObjectName);

      // Always tell if you want to update the 3D (vertex/index) later or not.
      bool lDoIWantToUpdateItLater = false;
      lManualObject->setDynamic(lDoIWantToUpdateItLater);

      // Here I create a cube in a first part with triangles, and then axes (in red/green/blue).

      // BaseWhiteNoLighting is the name of a material that already exist inside Ogre.
      // Ogre::RenderOperation::OT_TRIANGLE_LIST is a kind of primitive.
      float lSize = 0.7f;
      lManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
      {
        float cp = 1.0f * lSize;
        float cm = -1.0f * lSize;

        lManualObject->position(cm, cp, cm); // a vertex
        lManualObject->colour(Ogre::ColourValue(0.0f, 1.0f, 0.0f, 1.0f));
        lManualObject->position(cp, cp, cm); // a vertex
        lManualObject->colour(Ogre::ColourValue(1.0f, 1.0f, 0.0f, 1.0f));
        lManualObject->position(cp, cm, cm); // a vertex
        lManualObject->colour(Ogre::ColourValue(1.0f, 0.0f, 0.0f, 1.0f));
        lManualObject->position(cm, cm, cm); // a vertex
        lManualObject->colour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 1.0f));

        lManualObject->position(cm, cp, cp); // a vertex
        lManualObject->colour(Ogre::ColourValue(0.0f, 1.0f, 1.0f, 1.0f));
        lManualObject->position(cp, cp, cp); // a vertex
        lManualObject->colour(Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));
        lManualObject->position(cp, cm, cp); // a vertex
        lManualObject->colour(Ogre::ColourValue(1.0f, 0.0f, 1.0f, 1.0f));
        lManualObject->position(cm, cm, cp); // a vertex
        lManualObject->colour(Ogre::ColourValue(0.0f, 0.0f, 1.0f, 1.0f));

        // face behind / front
        lManualObject->triangle(0, 1, 2);
        lManualObject->triangle(2, 3, 0);
        lManualObject->triangle(4, 6, 5);
        lManualObject->triangle(6, 4, 7);

        // face top / down
        lManualObject->triangle(0, 4, 5);
        lManualObject->triangle(5, 1, 0);
        lManualObject->triangle(2, 6, 7);
        lManualObject->triangle(7, 3, 2);

        // face left / right
        lManualObject->triangle(0, 7, 4);
        lManualObject->triangle(7, 0, 3);
        lManualObject->triangle(1, 5, 6);
        lManualObject->triangle(6, 2, 1);
      }
      lManualObject->end();
      // Here I have finished my ManualObject construction.
      // It is possible to add more begin()-end() thing, in order to use
      // different rendering operation types, or different materials in 1 ManualObject.
      lManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
      {
        float lAxeSize = 2.0f * lSize;
        lManualObject->position(0.0f, 0.0f, 0.0f);
        lManualObject->colour(Ogre::ColourValue::Red);
        lManualObject->position(lAxeSize, 0.0f, 0.0f);
        lManualObject->colour(Ogre::ColourValue::Red);
        lManualObject->position(0.0f, 0.0f, 0.0f);
        lManualObject->colour(Ogre::ColourValue::Green);
        lManualObject->position(0.0, lAxeSize, 0.0);
        lManualObject->colour(Ogre::ColourValue::Green);
        lManualObject->position(0.0f, 0.0f, 0.0f);
        lManualObject->colour(Ogre::ColourValue::Blue);
        lManualObject->position(0.0, 0.0, lAxeSize);
        lManualObject->colour(Ogre::ColourValue::Blue);

        lManualObject->index(0);
        lManualObject->index(1);
        lManualObject->index(2);
        lManualObject->index(3);
        lManualObject->index(4);
        lManualObject->index(5);
      }
      lManualObject->end();
    }
    Ogre::String lNameOfTheMesh = "MeshCubeAndAxe";
    Ogre::String lResourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
    lManualObject->convertToMesh(lNameOfTheMesh);

    // Now I can create several entities using that mesh
    int lNumberOfEntities = 5;
    for(int iter = 0; iter < lNumberOfEntities; ++iter) {
      Ogre::Entity* lEntity = sceneManager_->createEntity(lNameOfTheMesh);
      // Now I attach it to a scenenode, so that it becomes present in the scene.
      Ogre::SceneNode* lNode = sceneManager_->getRootSceneNode()->createChildSceneNode();
      lNode->attachObject(lEntity);
      // I move the SceneNode so that it is visible to the camera.
      float lPositionOffset = float(1 + iter * 2) - (float(lNumberOfEntities));
      lNode->translate(lPositionOffset, lPositionOffset, -10.0f);
    }
  }
}

} // namespace game

} // namespace sequoia
