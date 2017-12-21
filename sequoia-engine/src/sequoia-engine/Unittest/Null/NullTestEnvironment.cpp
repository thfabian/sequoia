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

#include "sequoia-engine/Unittest/Null/NullTestEnvironment.h"

namespace sequoia {

namespace unittest {

NullTestEnvironment::NullTestEnvironment(int argc, char* argv[])
    : TestEnvironment(argc, argv, render::RK_Null) {}

void NullTestEnvironment::SetUp() {
  TestEnvironment::SetUp();

  auto options = getOptions();
  renderSystem_ = render::RenderSystem::create(render::RK_Null, options);
}

void NullTestEnvironment::TearDown() { renderSystem_.reset(); }

} // namespace unittest

} // namespace sequoia
