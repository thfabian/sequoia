//===-- sequoia/Game/Game.cpp -------------------------------------------------------*- C++ -*-===//
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

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Core/SmallVector.h"
#include "sequoia/Core/StringRef.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/RenderWindow.h"
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreWindowEventUtilities.h>

#include <OGRE/OgreManualObject.h>
#include <OGRE/OgreMesh.h>
#include <OGRE/OgreEntity.h>

template <>
sequoia::game::Game* Ogre::Singleton<sequoia::game::Game>::msSingleton = nullptr;

namespace sequoia {

namespace game {

Game::Game() {
  auto& config = GlobalConfiguration::getSingleton();

  //
  // Create root object
  //
  root_ = std::make_shared<Ogre::Root>(config.getPath("Game.PluginPath").string(),
                                       config.getPath("Game.ConfigPath").string(), "");

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
  renderSystem_ = std::make_shared<RenderSubsystem>(root_);
  renderSystem_->create(config.getBoolean("Game.ShowRenderDialog", false),
                        config.getString("CommandLine.RenderSystem", "OpenGL"));

  //
  // Initialize root
  //
  Ogre::String customCapacities = "";
  root_->initialise(false, "", customCapacities);

  //
  // Create RenderWindow
  //
  renderWindow_ = std::make_shared<RenderWindow>(root_);
  renderWindow_->create();


  // ----------------------------------------------

  Ogre::SceneManager* lScene = root_->createSceneManager(Ogre::ST_GENERIC, "MyFirstSceneManager");
	Ogre::SceneNode* lRootSceneNode = lScene->getRootSceneNode();
	Ogre::Camera* lCamera = lScene->createCamera("MyFirstCamera");

  // I attach the camera to a new SceneNode. It will be easier then to move it in the scene.
	Ogre::SceneNode* lCameraNode = lRootSceneNode->createChildSceneNode("MyFirstCameraNode");
	lCameraNode->attachObject(lCamera);

	// We create a viewport on a part of the window.
	// A viewport is the link between 1 camera and 1 drawing surface (here the window).
	// I can then call 'update();' on it to make it draw the Scene from the camera.
	// You can have several viewports on 1 window.
	// Check API for details on parameters.
	float lViewportWidth = 0.95f;
	float lViewportHeight = 0.95f;
	float lViewportLeft	= (1.0f - lViewportWidth) * 0.5f;
	float lViewportTop = (1.0f - lViewportHeight) * 0.5f;
	unsigned short lMainViewportZOrder = 100;
	Ogre::Viewport * vp = renderWindow_->get()->addViewport(lCamera, lMainViewportZOrder, lViewportLeft, lViewportTop, lViewportWidth, lViewportHeight);

	// I want the viewport to draw the scene automatically
	// when I will call lWindow->update();
	vp->setAutoUpdated(true);

	// I choose a color for this viewport. 
	// I prefer to have a bright color, to detect holes in geometry etc...
	vp->setBackgroundColour(Ogre::ColourValue(1,1,1));

	// I choose the visual ratio of the camera. To make it looks real, I want it the same as the viewport.
	float ratio = float(vp->getActualWidth()) / float(vp->getActualHeight());
	lCamera->setAspectRatio(ratio);

	// I choose the clipping far& near planes. if far/near>2000, you can get z buffer problem.
	// eg : far/near = 10000/5 = 2000 . it's ok.
	// If (far/near)>2000 then you will likely get 'z fighting' issues.
	lCamera->setNearClipDistance(1.5f);
	lCamera->setFarClipDistance(3000.0f); 

	// I want my window to be active
	renderWindow_->get()->setActive(true);

	// I want to update myself the content of the window, not automatically.
	renderWindow_->get()->setAutoUpdated(false);


	// A simplistic presentation of some realtime 3D basics.
	// In 3D you need most of the time : 
	// a vertex table Vt + an index table Idt + a kind of primitive P + an associated material M.
	// Let's say P is "triangle" (most common), then Idt will contain
	// a list of index taken from Vt, where each group of 3 index means a triangle.
	// so (Idt[0], Idt[1],Idt[2]) is the first triangle, (Idt[3],Idt[4],Idt[5]) is the second, etc...
	//
	// When times come to render this whole thing, 
	// it will be multiplied by 2 matrix.
	// 1/ a matrix representing transformation(position/orientation/scale) of the thing relative to the camera.
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
		// For each vertex I will provide positions and attributes (normal, vertex color, texture coordinates...).
		// Then for each primitive (given its type : triangle, line, line strip etc...), 
		// I give the corresponding group of vertex index.
		Ogre::ManualObject* lManualObject = NULL;
		{
			// The manualObject creation requires a name.
			Ogre::String lManualObjectName = "CubeWithAxes";
			lManualObject = lScene->createManualObject(lManualObjectName);
 
			// Always tell if you want to update the 3D (vertex/index) later or not.
			bool lDoIWantToUpdateItLater = false;
			lManualObject->setDynamic(lDoIWantToUpdateItLater);
 
			// Here I create a cube in a first part with triangles, and then axes (in red/green/blue).
 
			// BaseWhiteNoLighting is the name of a material that already exist inside Ogre.
			// Ogre::RenderOperation::OT_TRIANGLE_LIST is a kind of primitive.
			float lSize = 0.7f;
			lManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
			{
				float cp = 1.0f * lSize ;
				float cm = -1.0f * lSize;
 
				lManualObject->position(cm, cp, cm);// a vertex
				lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,0.0f,1.0f));
				lManualObject->position(cp, cp, cm);// a vertex
				lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,0.0f,1.0f));
				lManualObject->position(cp, cm, cm);// a vertex
				lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));
				lManualObject->position(cm, cm, cm);// a vertex
				lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));
 
				lManualObject->position(cm, cp, cp);// a vertex
				lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,1.0f,1.0f));
				lManualObject->position(cp, cp, cp);// a vertex
				lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));
				lManualObject->position(cp, cm, cp);// a vertex
				lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,1.0f,1.0f));
				lManualObject->position(cm, cm, cp);// a vertex
				lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,1.0f,1.0f));
 
				// face behind / front
				lManualObject->triangle(0,1,2);
				lManualObject->triangle(2,3,0);
				lManualObject->triangle(4,6,5);
				lManualObject->triangle(6,4,7);
 
				// face top / down
				lManualObject->triangle(0,4,5);
				lManualObject->triangle(5,1,0);
				lManualObject->triangle(2,6,7);
				lManualObject->triangle(7,3,2);
 
				// face left / right
				lManualObject->triangle(0,7,4);
				lManualObject->triangle(7,0,3);
				lManualObject->triangle(1,5,6);
				lManualObject->triangle(6,2,1);			
			}
			lManualObject->end();
			// Here I have finished my ManualObject construction.
			// It is possible to add more begin()-end() thing, in order to use 
			// different rendering operation types, or different materials in 1 ManualObject.
			lManualObject->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_LINE_LIST);
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
		for(int iter = 0; iter < lNumberOfEntities; ++iter)
		{
			Ogre::Entity* lEntity = lScene->createEntity(lNameOfTheMesh);
			// Now I attach it to a scenenode, so that it becomes present in the scene.
			Ogre::SceneNode* lNode = lRootSceneNode->createChildSceneNode();
			lNode->attachObject(lEntity);
			// I move the SceneNode so that it is visible to the camera.
			float lPositionOffset = float(1+ iter * 2) - (float(lNumberOfEntities));
			lNode->translate(lPositionOffset, lPositionOffset, -10.0f);
		}
	}

}

Game::~Game() {}

void Game::run() {
  root_->clearEventTimes();

  while(!renderWindow_->get()->isClosed()) {

    auto* window = renderWindow_->get();

    window->update(false);
		window->swapBuffers();

		root_->renderOneFrame();
    Ogre::WindowEventUtilities::messagePump();
  }
}

} // namespace game

} // namespace sequoia
