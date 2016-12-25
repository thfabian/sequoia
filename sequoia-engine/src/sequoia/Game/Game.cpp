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
  renderSystem_->create(config.getBoolean("Game.ShowRenderDialog", false));

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
}

Game::~Game() {}

void Game::run() {
  while(!renderWindow_->window()->isClosed()) {
    Ogre::WindowEventUtilities::messagePump();
  }
}

} // namespace game

} // namespace sequoia
