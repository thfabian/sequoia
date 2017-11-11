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
#include "sequoia-engine/Game/Game.h"
#include <iostream>

using namespace sequoia;

int main(int argc, char* argv[]) {
  std::shared_ptr<Options> options = Game::makeOptions();
  
  std::cout << options->toString() << std::endl;
//  core::CommandLine cl("sandbox", SANDBOX_VERSION);
  
  return 0;
}

