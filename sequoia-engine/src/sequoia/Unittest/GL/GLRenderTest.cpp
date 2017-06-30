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

#include "sequoia/Unittest/GL/GLRenderTest.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/RenderSystem.h"
#include "sequoia/Render/Viewport.h"

namespace sequoia {

namespace unittest {

render::GLRenderWindow* GLRenderTest::getWindow() const { return window_; }

void GLRenderTest::SetUp() {
  render::RenderWindow::WindowHint hints;
  hints.HideWindow = true;
  hints.WindowMode = render::RenderWindow::WindowHint::WK_Window;
  window_ = (render::GLRenderWindow*)render::RenderSystem::getSingleton().createMainWindow(hints);

  // Initialize Viewport
  auto viewport =
      std::make_shared<render::Viewport>(window_, 0, 0, window_->getWidth(), window_->getHeight());
  window_->setViewport(viewport);

  // Initialize OpenGL
  window_->init();
}

void GLRenderTest::TearDown() { render::RenderSystem::getSingleton().destroyMainWindow(); }

} // namespace unittest

} // namespace sequoia
