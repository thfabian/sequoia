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

#include "sequoia-engine/Render/D3D12/D3D12RenderSystem.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

TEST(D3D12RenderWindowTest, Construction) {
  RenderWindow::WindowHint hints;
  hints.HideWindow = true;
  hints.WindowMode = RenderWindow::WindowHint::WK_Window;
  hints.Width = 1280;
  hints.Height = 960;
  hints.Monitor = 1;
  RenderWindow* window = RenderSystem::getSingleton().createMainWindow(hints);

  // TODO: Remove
//  while(!window->isClosed()) {
//    RenderSystem::getSingleton().pollEvents();
//  }

  EXPECT_FALSE(window->isFullscreen());
  EXPECT_TRUE(window->isHidden());
  EXPECT_EQ(window->getWidth(), 1280);
  EXPECT_EQ(window->getHeight(), 960);

  RenderSystem::getSingleton().destroyMainWindow();
}

TEST(D3D12RenderWindowTest, InvalidMonitor) {
  RenderWindow::WindowHint hints;
  hints.Monitor = 999999;
  EXPECT_THROW(RenderSystem::getSingleton().createMainWindow(hints), RenderSystemException);
}

} // anonymous namespace
