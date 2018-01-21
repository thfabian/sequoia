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

#include "sequoia-engine/Render/D3D12/D3D12RenderWindow.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringUtil.h"

namespace sequoia {

namespace render {

D3D12RenderWindow::D3D12RenderWindow(const RenderWindow::WindowHint& hints)
    : RenderWindow(RK_D3D12RenderWindow) {
  Log::info("Initializing D3D12 window {} ...", core::ptrToStr(this));

  isHidden_ = hints.HideWindow;
  isFullscreen_ = hints.WindowMode == RenderWindow::WindowHint::WK_Fullscreen;
  isClosed_ = false;

  height_ = hints.Height;
  width_ = hints.Width;

  // Set a default view-port
  setViewport(std::make_shared<Viewport>(this, 0, 0, width_, height_));

  Log::info("Successfully initialized D3D12 window {}", core::ptrToStr(this));
}

D3D12RenderWindow::~D3D12RenderWindow() {
  Log::info("Terminating D3D12 window {} ...", core::ptrToStr(this));
  Log::info("Done terminating D3D12 window {}", core::ptrToStr(this));
}

bool D3D12RenderWindow::isHidden() { return isHidden_; }

bool D3D12RenderWindow::isClosed() { return isClosed_; }

bool D3D12RenderWindow::isFullscreen() const { return isFullscreen_; }

int D3D12RenderWindow::getWidth() const { return width_; }

int D3D12RenderWindow::getHeight() const { return height_; }

void D3D12RenderWindow::swapBuffers() {}

void D3D12RenderWindow::setCursorMode(RenderWindow::CursorModeKind mode) {}

void D3D12RenderWindow::centerCursor() {}

} // namespace render

} // namespace sequoia
