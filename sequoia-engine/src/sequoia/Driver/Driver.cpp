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

#include "sequoia/Driver/Driver.h"
#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Core/SingletonManager.h"
#include "sequoia/Driver/CommandLine.h"
#include "sequoia/Driver/ConsoleLogger.h"
#include "sequoia/Driver/Win32Console.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Core/PrettyStackTrace.h"

#ifdef SEQUOIA_ON_WIN32
#include <boost/program_options.hpp>
#endif

namespace sequoia {

namespace driver {

#ifdef SEQUOIA_ON_WIN32

int Driver::run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  // Define SingletonManager here to keep it alive
  auto singletonManager = std::make_unique<SingletonManager>();

  // Allocate OS specific Singletons
  TCHAR program[MAX_PATH];
  GetModuleFileName(NULL, program, MAX_PATH);
  singletonManager->allocateSingleton<ErrorHandler>(program);

  // Initialize options, parse config file and parse command-line
  singletonManager->allocateSingleton<Options>();

  // Initialize options, parse config file and parse command-line
  std::vector<std::string> arguments = boost::program_options::split_winmain(lpCmdLine);
  CommandLine::parse(arguments);

  return Driver::runImpl();
}

#else

int Driver::run(int argc, char* argv[]) {
  core::PrettyStackTrace sh;

  // Define SingletonManager here to keep it alive
  auto singletonManager = std::make_unique<SingletonManager>();

  // Allocate OS specific Singletons
  singletonManager->allocateSingleton<ErrorHandler>(argc > 0 ? argv[0] : "unknown");

  // Initialize options, parse config file and parse command-line
  singletonManager->allocateSingleton<Options>();

  // TODO: load options from config file
  
  std::vector<std::string> arguments(argv + 1, argv + argc);
  CommandLine::parse(arguments);
  
  return Driver::runImpl();
}

#endif

int Driver::runImpl() {
  SingletonManager& singletonManager = SingletonManager::getSingleton();
  Options& opt = Options::getSingleton();

  std::string level = opt.Driver.LoggingLevel;

  // Enable all options for debugging
  if(opt.Core.Debug) {
    opt.Driver.Logging = true;
    level = "Debug";
  }

  // Setup Logger
  singletonManager.allocateSingleton<core::Logger>(level);

  if(opt.Driver.Logging)
    singletonManager.allocateSingleton<ConsoleLogger>(opt.Driver.LoggingFile);

  // Setup game
  auto mainGameObject = std::make_unique<game::Game>();
  mainGameObject->init(false);
  mainGameObject->run();

  return 0;
}

} // namespace driver

} // namespace sequoia
