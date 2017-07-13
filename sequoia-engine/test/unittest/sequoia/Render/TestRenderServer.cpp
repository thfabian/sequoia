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

#include "sequoia/Render/RenderServer.h"
#include <gtest/gtest.h>

#include <chrono>

using namespace sequoia;
using namespace sequoia::render;

namespace {

TEST(RenderServerTest, SpawnRessourceTask) {
  RenderServer s;
  s.initRessourceThread([]() {});
  Future<int> f = s.spawnRessourceTask([]() {
    using namespace std::literals::chrono_literals;
    std::cout << "1" << std::endl;
    std::this_thread::sleep_for(1ms);
    std::cout << "2" << std::endl;
    return 1;
  });
  std::cout << f.get() << std::endl;
}

} // anonymous namespace
