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

#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Unittest/GL/GLEnvironment.h"
#include <gtest/gtest.h>

using namespace sequoia::render;

namespace {

TEST(GLRenderWindowTest, Construction) {
  RenderWindow::WindowHint hints;
  hints.HideWindow = true;
  hints.WindowMode = RenderWindow::WindowHint::WK_Window;
  hints.Width = 1280;
  hints.Height = 960;
  RenderWindow* window = RenderSystem::getSingleton().createWindow(hints);

  EXPECT_TRUE(window->isActive());
  EXPECT_FALSE(window->isFullscreen());
  EXPECT_FALSE(window->isHidden());
  EXPECT_EQ(window->getWidth(), 1280);
  EXPECT_EQ(window->getHeight(), 960);

  RenderSystem::getSingleton().destroyTarget(window);
}

} // anonymous namespace
