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

#include "sequoia-engine/Render/GL/GLRenderWindow.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/GL/GLRenderSystem.h"

namespace sequoia {

namespace render {

GLRenderWindow::GLRenderWindow(std::shared_ptr<NativeGLContext> context)
    : RenderWindow(RK_GLRenderWindow), nativeWindow_(nullptr), nativeInputSystem_(nullptr) {
  Log::info("Initializing OpenGL window {} ...", core::ptrToStr(this));

  nativeWindow_ = NativeWindow::create(context);
  nativeWindow_->addListener<NativeWindowListener>(this);
  nativeInputSystem_ = NativeInputSystem::create(nativeWindow_, true);

  // Set a default view-port
  setViewport(std::make_shared<Viewport>(this, 0, 0, getWidth(), getHeight()));

  // Register the window as an input event listener
  getGLRenderSystem().addListener<InputEventListener>(this);

  Log::info("Successfully initialized OpenGL window {}", core::ptrToStr(this));
}

GLRenderWindow::~GLRenderWindow() {
  Log::info("Terminating OpenGL window {} ...", core::ptrToStr(this));

  nativeInputSystem_.reset();
  nativeWindow_.reset();

  Log::info("Done terminating OpenGL window {}", core::ptrToStr(this));
}

bool GLRenderWindow::isHidden() { return nativeWindow_->isHidden(); }

bool GLRenderWindow::isClosed() { return nativeWindow_->isClosed(); }

bool GLRenderWindow::isFullscreen() const { return nativeWindow_->isFullscreen(); }

int GLRenderWindow::getWidth() const { return nativeWindow_->getWidth(); }

int GLRenderWindow::getHeight() const { return nativeWindow_->getHeight(); }

void GLRenderWindow::swapBuffers() { nativeWindow_->swapBuffers(); }

void GLRenderWindow::setCursorMode(RenderWindow::CursorModeKind mode) {
  nativeInputSystem_->setCursorMode(mode);
}

void GLRenderWindow::centerCursor() { nativeInputSystem_->centerCursor(); }

NativeInputSystem* GLRenderWindow::getInputSystem() { return nativeInputSystem_.get(); }

void GLRenderWindow::inputEventStart() {}

void GLRenderWindow::inputEventStop() {
  if(nativeInputSystem_->getCursorMode() == CK_Disabled)
    nativeInputSystem_->centerCursor();
}

void GLRenderWindow::nativeWindowGeometryChanged(NativeWindow* window) {
  Log::info("Resizing window {} to {} x {}", core::ptrToStr(this), window->getWidth(),
            window->getHeight());
  Viewport* viewport = getViewport();
  viewport->updateGeometry(viewport->getX(), viewport->getY(), window->getWidth(),
                           window->getHeight());
}

void GLRenderWindow::nativeWindowFocusChanged(NativeWindow* window) {}

NativeGLContext* GLRenderWindow::getContext() const { return nativeWindow_->getContext(); }

} // namespace render

} // namespace sequoia
