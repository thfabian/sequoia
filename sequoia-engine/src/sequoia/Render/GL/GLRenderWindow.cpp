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

#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include <unordered_map>

namespace sequoia {

namespace render {

std::unordered_map<GLFWwindow*, GLRenderWindow*> GLRenderWindow::StaticWindowMap;

GLRenderWindow::GLRenderWindow(const std::string& title)
    : RenderWindow(RK_GLRenderWindow), window_(nullptr), isFullscreen_(false), windowWidth_(-1),
      windowHeight_(-1) {
  LOG(INFO) << "Initializing window " << this << " ...";

  Options& opt = Options::getSingleton();

  // Use atleast OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opt.Render.GLMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opt.Render.GLMinorVersion);

  // Set Antialiasing
  glfwWindowHint(GLFW_SAMPLES, opt.Render.FSAA);
  LOG(INFO) << "Using FSAA: " << opt.Render.FSAA;

  // Specifies whether the OpenGL context should be forward-compatible, i.e. one where all
  // functionality deprecated in the requested version of OpenGL is removed
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Select the monitor to use
  GLFWmonitor* monitor = nullptr;
  if(opt.Render.Monitor == -1) {
    monitor = glfwGetPrimaryMonitor();
  } else {
    int numMonitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);
    SEQUOIA_ASSERT(monitors);

    if(opt.Render.Monitor >= numMonitors)
      SEQUOIA_THROW(RenderSystemInitException,
                    "invalid monitor '%i' (max number of monitors is %i)", opt.Render.Monitor,
                    numMonitors);
    monitor = monitors[opt.Render.Monitor];
  }

  LOG(INFO) << "Using monitor " << glfwGetMonitorName(monitor);
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  // Select the window-mode
  if(opt.Render.WindowMode == "fullscreen") {
    window_ = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);
    isFullscreen_ = true;
  } else {

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    if(opt.Render.WindowMode == "window") {
      window_ = glfwCreateWindow(1280, 960, title.c_str(), nullptr, nullptr);
    } else if(opt.Render.WindowMode == "windowed-fullscreen") {
      window_ = glfwCreateWindow(mode->width, mode->height, title.c_str(), nullptr, nullptr);
    } else
      sequoia_unreachable("invalid window-mode");
  }
  LOG(INFO) << "Using window mode: " << opt.Render.WindowMode;

  if(!window_)
    SEQUOIA_THROW(RenderSystemInitException,
                  "failed to initialize GLFW window, requested OpenGL (%i. %i)",
                  opt.Render.GLMajorVersion, opt.Render.GLMinorVersion);

  // Move the window to the correct monitor (fullscreen windows are already moved correctly)
  if(!isFullscreen_) {
    int xpos, ypos;
    glfwGetMonitorPos(monitor, &xpos, &ypos);

    if(opt.Render.WindowMode == "window")
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

  // Register the window "globally"
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

  LOG(INFO) << "Done terminating window" << this;
}

bool GLRenderWindow::isClosed() { return glfwWindowShouldClose(window_); }

bool GLRenderWindow::isFullscreen() const { return isFullscreen_; }

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

void GLRenderWindow::init() { renderer_ = std::make_shared<GLRenderer>(this); }

GLFWwindow* GLRenderWindow::getGLFWwindow() { return window_; }

const std::shared_ptr<GLRenderer>& GLRenderWindow::getRenderer() const { return renderer_; }

} // namespace render

} // namespace sequoia
