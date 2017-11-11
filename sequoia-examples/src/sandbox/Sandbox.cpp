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

#include "sandbox/Config.h"
#include "sequoia-engine/Core/CommandLine.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/PrettyStackTrace.h"
#include "sequoia-engine/Game/Game.h"
#include <iostream>

using namespace sequoia;

int main(int argc, char* argv[]) {
  // Print a pretty stack trace if we signal out
  core::PrettyStackTrace trace;

  // Enable logging to stdout
  auto logger = std::make_unique<core::Logger>();

  // Parse options from config file and command-line
  std::shared_ptr<Options> options = Game::makeOptions();
  options->read("Sandbox-Config.xml");

  core::CommandLine cl("Sequoia Sandbox Example", SEQUOIA_EXAMPLES_SANDBOX_VERSION);
  cl.parse(options.get(), argc, argv);

  // Write options to file
  options->write("Sandbox-Config.xml");
  
  return 0;
}
