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
#include "sequoia/Game/RenderSubsystem.h"
#include "sequoia/Game/RenderWindow.h"
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreWindowEventUtilities.h>

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
  RenderSubsystem rendersystem(root_, config.getBoolean("Game.ShowRenderDialog", false));

  //
  // Initialize root
  //
  Ogre::String customCapacities = "";
  root_->initialise(false, "", customCapacities);

  //
  // Create RenderWindow
  //
  Ogre::String windowTitle = "Hello Ogre World";
  unsigned int sizeX = 800;
  unsigned int sizeY = 600;
  // I don't want to use fullscreen during development.
  bool fullscreen = false;
  // This is just an example of parameters that we can put. Check the API for more details.
  Ogre::NameValuePairList params;
  // fullscreen antialiasing. (check wikipedia if needed).
  params["FSAA"] = "0";
  // vertical synchronisation will prevent some image-tearing, but also
  // will provide smooth framerate in windowed mode.(check wikipedia if needed).
  params["vsync"] = "true";
  renderWindow_ = root_->createRenderWindow(windowTitle, sizeX, sizeY, fullscreen, &params);
}

Game::~Game() {}

void Game::run() {
  while(!renderWindow_->isClosed()) {
    Ogre::WindowEventUtilities::messagePump();
  }
}

} // namespace game

} // namespace sequoia
