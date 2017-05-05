//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Core/SingletonManager.h"
#include "sequoia/Driver/CommandLine.h"
#include "sequoia/Driver/ConsoleLogger.h"
#include "sequoia/Driver/Driver.h"
#include "sequoia/Driver/Win32Console.h"

namespace sequoia {

namespace driver {

#ifdef SEQUOIA_ON_WIN32

int Driver::run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  //  // Define SingletonManager here to keep it alive
  //  auto singletonManager = std::make_unique<SingletonManager>();

  //  // Allocate OS specific Singletons
  //  TCHAR program[MAX_PATH];
  //  GetModuleFileName(NULL, program, MAX_PATH);
  //  singletonManager->allocateSingleton<ErrorHandler>(program);

  //  // Initialize options, parse config file and parse command-line
  //  std::vector<std::string> arguments = boost::program_options::split_winmain(lpCmdLine);
  //  CommandLine::parse(arguments);

  //  return Driver::runImpl();
}

#else

int Driver::run(int argc, char* argv[]) {
  // Define SingletonManager here to keep it alive
  auto singletonManager = std::make_unique<SingletonManager>();

  // Allocate OS specific Singletons
  singletonManager->allocateSingleton<ErrorHandler>(argc > 0 ? argv[0] : "unknown");

  // Initialize options, parse config file and parse command-line
  singletonManager->allocateSingleton<Options>();

  std::vector<std::string> arguments(argv + 1, argv + argc);
  CommandLine::parse(arguments);

  return Driver::runImpl();
}

#endif

int Driver::runImpl() {
  SingletonManager& singletonManager = SingletonManager::getSingleton();
  Options& opt = Options::getSingleton();

  // Enable all options for debugging
  if(opt.Core.Debug) {
    opt.Driver.Logging = true;
  }

  // Setup Logger
  singletonManager.allocateSingleton<core::Logger>();

  if(opt.Driver.Logging)
    singletonManager.allocateSingleton<ConsoleLogger>(opt.Driver.LoggingFile);

  LOG(INFO) << "does it work?";

  //  // Setup & run game
  //  try {
  //    SingletonManager::getSingleton().allocateSingleton<game::Game>();
  //    game::Game::getSingleton().run();
  //  } catch(std::exception& e) {
  //    core::ErrorHandler::getSingleton().fatal(e.what());
  //    return 1;
  //  }

  //  // Save configuration to disk (create the config path if necessary)
  //  try {
  //    config.save(config.getPath("CommandLine.ConfigFile", CSTR(SEQUOIA_GLOBAL_CONFIG_PATH)));
  //  } catch(std::exception& e) {
  //    core::ErrorHandler::getSingleton().warning(e.what());
  //  }

  return 0;
}

} // namespace driver

} // namespace sequoia
