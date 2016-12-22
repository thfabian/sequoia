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

#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Core/SmallVector.h"
#include "sequoia/Core/StringRef.h"
#include "sequoia/Game/Game.h"
#include <OGRE/OgreRenderSystem.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRoot.h>
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
}

Game::~Game() {}

void Game::run() {}

} // namespace game

} // namespace sequoia
