//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine (2016-2017)
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia-engine/Core/CommandLine.h"
#include "sequoia-engine/Core/ErrorHandler.h"
#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/PrettyStackTrace.h"
#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Game/Keymap.h"
#include "sequoia-examples/Sandbox/Config.h"
#include "sequoia-examples/Sandbox/SandboxScene.h"
#include <iostream>

using namespace sequoia;
using namespace sequoia_examples;

int main(int argc, char* argv[]) {
  // Print a pretty stack trace if we signal out
  core::PrettyStackTrace trace;

  // Enable logging to stdout
  auto logger = std::make_unique<core::Logger>();

  // Parse options from config file and command-line
  std::shared_ptr<Options> options = game::Game::makeOptions();
  //options->read("Sandbox-Config.xml");
  options->setString("Game.RessourcePath", SEQUOIA_EXAMPLES_SANDBOX_RESSOURCEPATH);

  core::CommandLine cl("Sequoia Sandbox Example", SEQUOIA_EXAMPLES_SANDBOX_VERSION);
  cl.parse(options, argc, argv);

  if(options->getBool("Core.Debug"))
    logger->setLevel(core::Logger::Trace);

  // Run the game
  options->setString("Game.Name", "Sequoia Sandbox Example");
  try {
    auto sandboxGame = std::make_unique<game::Game>();
    sandboxGame->init(options);

    sandboxGame->setQuitKey(game::Keymap::makeDefault(render::Key_Q, render::Mod_Ctrl));
    sandboxGame->setScene(std::make_shared<sandbox::SandboxScene>("SandboxScene"));
    sandboxGame->run();

  } catch(core::Exception& exception) {
    core::ErrorHandler::fatal(exception.what());
  }

  // Write options to file
  // options->write("Sandbox-Config.xml");

  return 0;
}
