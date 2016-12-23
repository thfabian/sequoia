//===-- sequoia/Driver/Driver.cpp ---------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Setup main-loop of the framework.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/GlobalConfiguration.h"
#include "sequoia/Core/SingletonManager.h"
#include "sequoia/Driver/CommandLine.h"
#include "sequoia/Driver/Driver.h"
#include "sequoia/Game/Game.h"
#include <OGRE/OgreLogManager.h>

#ifdef SEQUOIA_ON_WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace sequoia {

namespace driver {

#ifdef SEQUOIA_ON_WIN32

int Driver::run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  // Define SingletonManager here to keep it alive
  auto singletonManager = std::make_unique<SingletonManager>();
  Driver::setDefaultConfigs();
  
  // Allocate OS specific Singletons
  auto& singletonManager = SingletonManager::getSingleton();
  
  TCHAR program[MAX_PATH];
  GetModuleFileName(NULL, program, MAX_PATH);
  singletonManager->allocateSingleton<ErrorHandler>(program);
  
  // Parse command-line
  // TODO...
  
  return Driver::runImpl();
}

#else

int Driver::run(int argc, char* argv[]) {
  // Define SingletonManager here to keep it alive
  auto singletonManager = std::make_unique<SingletonManager>();
  Driver::setDefaultConfigs();

  // Allocate OS specific Singletons
  singletonManager->allocateSingleton<ErrorHandler>(argc > 0 ? argv[0] : "unknown");
  
  // Parse command-line
  std::vector<DefaultString> arguments(argv + 1, argv + argc);
  CommandLine::parse(arguments);

  return Driver::runImpl();
}

#endif

void Driver::setDefaultConfigs() {
  auto& singletonManager = SingletonManager::getSingleton();
  singletonManager.allocateSingleton<GlobalConfiguration>();
  
  auto& config = GlobalConfiguration::getSingleton();  
  config.put("Game.PluginPath", boost::filesystem::path(CSTR(SEQUOIA_OGRE_CONFIG_PATH)));
  config.put("Game.ConfigPath", boost::filesystem::path(CSTR(SEQUOIA_OGRE_CONFIG_PATH)));
}

int Driver::runImpl() {
  auto& config = GlobalConfiguration::getSingleton();  
  
  // Create Logger TODO: should be deleted last
  auto* lm = new Ogre::LogManager();
  lm->createLog("sequoia.log", true, true, false);

  // Setup & run game
  try {
    SingletonManager::getSingleton().allocateSingleton<game::Game>();
    game::Game::getSingleton().run();
  } catch(std::exception& e) {
    core::ErrorHandler::getSingleton().fatal(e.what());
    return 1;
  }

  // Save configuration to disk (create the config path if necessary)
  try {
    config.save(config.getPath("CommandLine.ConfigFile", CSTR(SEQUOIA_GLOBAL_CONFIG_PATH)));
  } catch(std::exception& e) {
    core::ErrorHandler::getSingleton().warning(e.what());
  }

  return 0;
}

} // namespace driver

} // namespace sequoia