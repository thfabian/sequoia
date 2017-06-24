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

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLInputSystem.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include <unordered_map>

namespace sequoia {

namespace render {

std::unordered_map<GLFWwindow*, GLRenderWindow*> GLRenderWindow::StaticWindowMap;

GLRenderWindow::GLRenderWindow(GLRenderSystem* renderSystem,
                               const RenderWindow::WindowHint& windowHints)
    : RenderWindow(RK_GLRenderWindow), renderSystem_(renderSystem), window_(nullptr),
      windowWidth_(-1), windowHeight_(-1), mode_(CursorModeKind::CK_Normal), renderer_(nullptr),
      inputSystem_(nullptr) {
  Options& opt = Options::getSingleton();

  LOG(INFO) << "Initializing OpenGL window " << this << " ...";

  // Open the window hidden?
  if(windowHints.HideWindow) {
    LOG(INFO) << "Using hidden window";
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  }

  // Set Antialiasing
  glfwWindowHint(GLFW_SAMPLES, opt.Render.MSAA);
  LOG(INFO) << "Using MSAA: " << opt.Render.MSAA;

  // Specifies whether the OpenGL context should be forward-compatible, i.e. one where all
  // functionality deprecated in the requested version of OpenGL is removed
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if(RenderSystem::getSingleton().debugMode()) {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  }

  // Select the monitor to use
  GLFWmonitor* monitor = nullptr;
  if(windowHints.Monitor == -1) {
    monitor = glfwGetPrimaryMonitor();
  } else {
    int numMonitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&numMonitors);
    SEQUOIA_ASSERT(monitors);

    if(windowHints.Monitor >= numMonitors) {
      LOG(WARNING) << "invalid monitor '" << windowHints.Monitor << "' (max number of monitors is "
                   << numMonitors << ")";
      monitor = glfwGetPrimaryMonitor();
    } else {
      monitor = monitors[windowHints.Monitor];
    }
  }

  LOG(INFO) << "Using monitor " << glfwGetMonitorName(monitor);
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  LOG(INFO) << "Using window mode: " << windowHints.WindowMode;

  // Select the window-mode
  auto createWindow = [&](bool throwOnError, int major, int minor) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

    LOG(INFO) << "Attempting to initialize GLFW window, requesting OpenGL (" << major << "."
              << minor << ")";

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
        window_ = glfwCreateWindow(mode->width, mode->height, windowHints.Title.c_str(), nullptr,
                                   nullptr);
      } else
        sequoia_unreachable("invalid window-mode");
    }

    if(throwOnError && !window_)
      SEQUOIA_THROW(RenderSystemException,
                    "failed to initialize GLFW window, required atleast OpenGL Core (>= 3.3)");
  };

  createWindow(false, opt.Render.GLMajorVersion, opt.Render.GLMinorVersion);
  if(!window_) {
    LOG(WARNING) << "Failed to initialize GLFW window, requested OpenGL ("
                 << opt.Render.GLMajorVersion << "." << opt.Render.GLMinorVersion << ")";

    // We need atleast OpenGL 3.3
    createWindow(true, 3, 3);
  }

  LOG(INFO) << "Sucessfuly initialized GLFW window";

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

  LOG(INFO) << "Done initializing OpenGL window " << this;
}

void GLRenderWindow::init() {
  SEQUOIA_ASSERT_MSG(hasViewport(), "RenderTarget::init() called with no Viewport set");

  renderer_ = std::make_unique<GLRenderer>(this);
  renderSystem_->registerRenderer(this, renderer_.get());

  LOG(INFO) << "Registering IO callbacks ...";

  inputSystem_ = std::make_unique<GLInputSystem>(this, true);
  renderSystem_->registerInputSystem(this, inputSystem_.get());

  glfwSetInputMode(window_, GLFW_STICKY_KEYS, 1);
  glfwSetKeyCallback(window_, GLRenderWindow::keyCallbackDispatch);
  glfwSetMouseButtonCallback(window_, GLRenderWindow::mouseButtonCallbackDispatch);
  glfwSetCursorPosCallback(window_, GLRenderWindow::mousePositionCallbackDispatch);
  glfwSetCursorEnterCallback(window_, GLRenderWindow::mouseEnterCallbackDispatch);

  renderSystem_->addListener<InputEventListener>(static_cast<InputEventListener*>(this));

  LOG(INFO) << "Done registering IO callbacks";
}

GLRenderWindow::~GLRenderWindow() {
  LOG(INFO) << "Terminating OpenGL window " << this << " ...";

  renderer_.reset();
  inputSystem_.reset();

  StaticWindowMap.erase(window_);
  glfwDestroyWindow(window_);

  LOG(INFO) << "Done terminating OpenGL window " << this;
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

void GLRenderWindow::keyCallbackDispatch(GLFWwindow* window, int key, int scancode, int action,
                                         int mods) {
  GLRenderWindow::StaticWindowMap[window]->getInputSystem()->keyCallback(key, action, mods);
}

void GLRenderWindow::mouseButtonCallbackDispatch(GLFWwindow* window, int button, int action,
                                                 int mods) {
  GLRenderWindow::StaticWindowMap[window]->getInputSystem()->mouseButtonCallback(button, action,
                                                                                 mods);
}

void GLRenderWindow::mousePositionCallbackDispatch(GLFWwindow* window, double xpos, double ypos) {
  GLRenderWindow::StaticWindowMap[window]->getInputSystem()->mousePositionCallback(xpos, ypos);
}

void GLRenderWindow::mouseEnterCallbackDispatch(GLFWwindow* window, int entered) {
  if(entered) {
    GLRenderWindow* glWindow = GLRenderWindow::StaticWindowMap[window];
    if(glWindow->getCursorMode() == RenderTarget::CursorModeKind::CK_Disabled)
      glWindow->getInputSystem()->centerCursor();
  }
}

void GLRenderWindow::inputEventStart() {}

void GLRenderWindow::inputEventStop() {
  if(mode_ == CK_Disabled)
    getInputSystem()->centerCursor();
}

int GLRenderWindow::getWidth() const { return windowWidth_; }

int GLRenderWindow::getHeight() const { return windowHeight_; }

void GLRenderWindow::swapBuffers() { glfwSwapBuffers(window_); }

void GLRenderWindow::update() { renderer_->render(); }

void GLRenderWindow::setCursorMode(RenderTarget::CursorModeKind mode) {
  mode_ = mode;
  switch(mode_) {
  case CK_Disabled:
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
  case CK_Hidden:
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    break;
  case CK_Normal:
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  }
}

void GLRenderWindow::centerCursor() { getInputSystem()->centerCursor(); }

GLFWwindow* GLRenderWindow::getGLFWwindow() { return window_; }

RenderTarget::CursorModeKind GLRenderWindow::getCursorMode() { return mode_; }

GLRenderer* GLRenderWindow::getRenderer() { return renderer_.get(); }

GLInputSystem* GLRenderWindow::getInputSystem() { return inputSystem_.get(); }

} // namespace render

} // namespace sequoia
