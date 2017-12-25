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

#include "sequoia-engine/Render/RenderSystem.h"
#include "sequoia-engine/Render/RenderWindow.h"
#include "sequoia-engine/Unittest/RenderSetup.h"

namespace sequoia {

namespace unittest {

void RenderSetup::SetUp() {
  render::RenderWindow::WindowHint hints;
  hints.HideWindow = true;
  hints.WindowMode = render::RenderWindow::WindowHint::WK_Window;
  render::RenderSystem::getSingleton().createMainWindow(hints);
}

void RenderSetup::TearDown() { render::RenderSystem::getSingleton().destroyMainWindow(); }

} // namespace unittest

} // namespace sequoia
