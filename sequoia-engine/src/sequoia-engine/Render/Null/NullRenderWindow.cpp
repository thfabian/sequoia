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

#include "sequoia-engine/Render/Null/NullRenderWindow.h"
#include "sequoia-engine/Core/Logging.h"

namespace sequoia {

namespace render {

NullRenderWindow::NullRenderWindow(const RenderWindow::WindowHint& hints)
    : RenderWindow(RK_NullRenderWindow) {
  LOG(INFO) << "Initializing Null window " << this << " ...";

  isHidden_ = hints.HideWindow;
  isFullscreen_ = hints.WindowMode == RenderWindow::WindowHint::WK_Fullscreen;
  isClosed_ = false;

  height_ = hints.Height;
  width_ = hints.Width;

  // Set a default view-port
  setViewport(std::make_shared<Viewport>(this, 0, 0, width_, height_));

  LOG(INFO) << "Successfully initialized Null window " << this;
}

NullRenderWindow::~NullRenderWindow() {
  LOG(INFO) << "Terminating Null window " << this << " ...";
  LOG(INFO) << "Done terminating Null window " << this;
}

bool NullRenderWindow::isHidden() { return isHidden_; }

bool NullRenderWindow::isClosed() { return isClosed_; }

bool NullRenderWindow::isFullscreen() const { return isFullscreen_; }

int NullRenderWindow::getWidth() const { return width_; }

int NullRenderWindow::getHeight() const { return height_; }

void NullRenderWindow::swapBuffers() {}

void NullRenderWindow::setCursorMode(RenderWindow::CursorModeKind mode) {}

void NullRenderWindow::centerCursor() {}

} // namespace render

} // namespace sequoia
