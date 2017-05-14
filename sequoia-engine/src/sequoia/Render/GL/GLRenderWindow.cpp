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

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include <unordered_map>

#include <iostream>

namespace sequoia {

namespace render {

std::unordered_map<GLFWwindow*, GLRenderWindow*> GLRenderWindow::StaticWindowMap;

GLRenderWindow::GLRenderWindow(GLRenderSystem* renderSystem,
                               const RenderWindow::WindowHint& windowHints)
    : RenderWindow(RK_GLRenderWindow), renderSystem_(renderSystem), window_(nullptr),
      windowWidth_(-1), windowHeight_(-1) {
  LOG(INFO) << "Initializing window " << this << " ...";

  // Open the window hidden?
  if(windowHints.HideWindow) {
    LOG(INFO) << "Using hidden window";
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  }

  // Use atleast OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowHints.GLMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowHints.GLMinorVersion);

  // Set Antialiasing
  glfwWindowHint(GLFW_SAMPLES, windowHints.FSAA);
  LOG(INFO) << "Using FSAA: " << windowHints.FSAA;

  // Specifies whether the OpenGL context should be forward-compatible, i.e. one where all
  // functionality deprecated in the requested version of OpenGL is removed
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Select the monitor to use
  GLFWmonitor* monitor = nullptr;
  if(windowHints.Monitor == -1) {
    monitor = glfwGetPrimaryMonitor();
  } else {
    int numMonitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);
    SEQUOIA_ASSERT(monitors);

    if(windowHints.Monitor >= numMonitors)
      SEQUOIA_THROW(RenderSystemException, "invalid monitor '%i' (max number of monitors is %i)",
                    windowHints.Monitor, numMonitors);
    monitor = monitors[windowHints.Monitor];
  }

  LOG(INFO) << "Using monitor " << glfwGetMonitorName(monitor);
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  // Select the window-mode
  if(windowHints.WindowMode == WindowHint::WK_Fullscreen) {
    window_ =
        glfwCreateWindow(mode->width, mode->height, windowHints.Title.c_str(), monitor, nullptr);
  } else {

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    if(windowHints.WindowMode == WindowHint::WK_Window) {
      window_ = glfwCreateWindow(windowHints.Width, windowHints.Height, windowHints.Title.c_str(),
                                 nullptr, nullptr);
    } else if(windowHints.WindowMode == WindowHint::WK_WindowedFullscreen) {
      window_ =
          glfwCreateWindow(mode->width, mode->height, windowHints.Title.c_str(), nullptr, nullptr);
    } else
      sequoia_unreachable("invalid window-mode");
  }
  LOG(INFO) << "Using window mode: " << windowHints.WindowMode;

  if(!window_)
    SEQUOIA_THROW(RenderSystemException,
                  "failed to initialize GLFW window, requested OpenGL (%i. %i)",
                  windowHints.GLMajorVersion, windowHints.GLMinorVersion);

  // Move the window to the correct monitor (fullscreen windows are already moved correctly)
  if(windowHints.WindowMode != WindowHint::WK_Fullscreen) {
    int xpos, ypos;
    glfwGetMonitorPos(monitor, &xpos, &ypos);

    if(windowHints.WindowMode == WindowHint::WK_Window)
      glfwSetWindowPos(window_, xpos + 30, ypos + 60);
    else {
      glfwSetWindowPos(window_, xpos, ypos);
      glfwRestoreWindow(window_);
      glfwMaximizeWindow(window_);
    }
  }

  // Query width and height
  glfwGetWindowSize(window_, &windowWidth_, &windowHeight_);
  LOG(INFO) << "Using window geometry: " << windowWidth_ << " x " << windowHeight_;

  // Register the window "globally" (requred for GLFW callbacks)
  StaticWindowMap.emplace(window_, this);

  // Register window call-back
  glfwSetWindowSizeCallback(window_, GLRenderWindow::resizeCallbackDispatch);

  LOG(INFO) << "Done initializing window " << this;
}

GLRenderWindow::~GLRenderWindow() {
  LOG(INFO) << "Terminating window " << this << " ...";

  renderer_.reset();
  StaticWindowMap.erase(window_);
  glfwDestroyWindow(window_);

  LOG(INFO) << "Done terminating window " << this;
}

bool GLRenderWindow::isHidden() { return glfwGetWindowAttrib(window_, GLFW_VISIBLE); }

bool GLRenderWindow::isClosed() { return glfwWindowShouldClose(window_); }

bool GLRenderWindow::isFullscreen() const { return glfwGetWindowMonitor(window_) != nullptr; }

void GLRenderWindow::resizeCallback(int width, int height) {
  LOG(INFO) << "Resizing window " << this << " to " << width << " x " << height;
  windowWidth_ = width;
  windowHeight_ = height;
  Viewport* viewport = getViewport();
  viewport->updateGeometry(viewport->getX(), viewport->getY(), windowWidth_, windowHeight_);
}

void GLRenderWindow::resizeCallbackDispatch(GLFWwindow* window, int width, int height) {
  GLRenderWindow::StaticWindowMap[window]->resizeCallback(width, height);
}

int GLRenderWindow::getWidth() const { return windowWidth_; }

int GLRenderWindow::getHeight() const { return windowHeight_; }

void GLRenderWindow::swapBuffers() { glfwSwapBuffers(window_); }

void GLRenderWindow::update() { renderer_->render(); }

void GLRenderWindow::init() {
  renderer_ = std::make_unique<GLRenderer>(this);
  renderSystem_->registerRenderer(this, renderer_.get());
}

GLFWwindow* GLRenderWindow::getGLFWwindow() { return window_; }

GLRenderer* GLRenderWindow::getRenderer() { return renderer_.get(); }

} // namespace render

} // namespace sequoia
