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

#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Options.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/NativeGLFW3.h"
#include "sequoia/Render/RenderSystem.h"
#include <GLFW/glfw3.h>

namespace sequoia {

namespace render {

//===------------------------------------------------------------------------------------------===//
//    Context
//===------------------------------------------------------------------------------------------===//

static void CallbackErrorSoft(int error, const char* description) {
  LOG(ERROR) << "glfw3: error: " << description;
}

static void CallbackErrorHard(int error, const char* description) {
  CallbackErrorSoft(error, description);
  SEQUOIA_THROW(RenderSystemException, description);
}

int glfw3NativeGLContext::NumContexts = 0;

std::unordered_map<GLFWwindow*, glfw3NativeGLContext*> glfw3NativeGLContext::ContextMap;

glfw3NativeGLContext::glfw3NativeGLContext()
    : NativeGLContext(NK_GLFW3), window_(nullptr), parent_(nullptr) {
  if(glfw3NativeGLContext::NumContexts == 0) {
    LOG(INFO) << "Initializing glfw3 ... ";
    glfwSetErrorCallback(CallbackErrorHard);

    glfwInit();

    glfwSetErrorCallback(CallbackErrorSoft);
    LOG(INFO) << "Successfully initialized glfw3: " << glfwGetVersionString();
  }
  glfw3NativeGLContext::NumContexts++;
}

glfw3NativeGLContext::~glfw3NativeGLContext() {
  glfw3NativeGLContext::NumContexts--;

  if(window_) {
    glfwDestroyWindow(window_);
    glfw3NativeGLContext::ContextMap.erase(window_);
  }

  if(glfw3NativeGLContext::NumContexts == 0) {
    glfwTerminate();
  }
}

void glfw3NativeGLContext::init(const RenderWindow::WindowHint& windowHints) {
  Options& opt = Options::getSingleton();

  LOG(INFO) << "Initializing glfw3 OpenGL context " << this << " ...";

  LOG(INFO) << "Setting window hints ...";

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

  LOG(INFO) << "Using monitor: " << glfwGetMonitorName(monitor);
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  LOG(INFO) << "Using window mode: " << windowHints.WindowMode;

  // Select the window-mode
  auto createWindow = [&](bool throwOnError, int major, int minor) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

    LOG(INFO) << "Attempting to initialize glfw3 window, requesting OpenGL (" << major << "."
              << minor << ")";

    if(windowHints.WindowMode == RenderWindow::WindowHint::WK_Fullscreen) {
      window_ =
          glfwCreateWindow(mode->width, mode->height, windowHints.Title.c_str(), monitor, nullptr);
    } else {
      glfwWindowHint(GLFW_RED_BITS, mode->redBits);
      glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
      glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
      glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

      if(windowHints.WindowMode == RenderWindow::WindowHint::WK_Window) {
        window_ = glfwCreateWindow(windowHints.Width, windowHints.Height, windowHints.Title.c_str(),
                                   nullptr, nullptr);
      } else if(windowHints.WindowMode == RenderWindow::WindowHint::WK_WindowedFullscreen) {
        window_ = glfwCreateWindow(mode->width, mode->height, windowHints.Title.c_str(), nullptr,
                                   nullptr);
      } else
        sequoia_unreachable("invalid window-mode");
    }

    if(throwOnError && !window_)
      SEQUOIA_THROW(RenderSystemException,
                    "failed to initialize glfw3 window, required atleast OpenGL Core (>= 3.3)");
  };

  createWindow(false, opt.Render.GLMajorVersion, opt.Render.GLMinorVersion);
  if(!window_) {
    LOG(WARNING) << "Failed to initialize glfw3 window, requested OpenGL ("
                 << opt.Render.GLMajorVersion << "." << opt.Render.GLMinorVersion << ")";

    // We need atleast OpenGL 3.3
    createWindow(true, 3, 3);
  }

  // Move the window to the correct monitor (fullscreen windows are already moved correctly)
  if(windowHints.WindowMode != RenderWindow::WindowHint::WK_Fullscreen) {
    int xpos, ypos;
    glfwGetMonitorPos(monitor, &xpos, &ypos);

    if(windowHints.WindowMode == RenderWindow::WindowHint::WK_Window)
      glfwSetWindowPos(window_, xpos + 30, ypos + 60);
    else {
      glfwSetWindowPos(window_, xpos, ypos);
      glfwRestoreWindow(window_);
      glfwMaximizeWindow(window_);
    }
  }

  // Register the context
  glfw3NativeGLContext::ContextMap.emplace(window_, this);

  LOG(INFO) << "Successfully initialized glfw3 OpenGL context " << this << " ...";
}

void glfw3NativeGLContext::init(const std::shared_ptr<NativeGLContext>& context) {}

glfw3NativeGLContext* glfw3NativeGLContext::getNativeGLContext(GLFWwindow* window) {
  return ContextMap[window];
}

//===------------------------------------------------------------------------------------------===//
//    Window
//===------------------------------------------------------------------------------------------===//

static void CallbackResized(GLFWwindow* window, int width, int height) {
  glfw3NativeWindow::getNativeWindow(window)->setGeometry(width, height);
}

static void CallbackFocused(GLFWwindow* window, int focus) {
  glfw3NativeWindow::getNativeWindow(window)->setFocus(focus);
}

std::unordered_map<GLFWwindow*, glfw3NativeWindow*> glfw3NativeWindow::WindowMap;

glfw3NativeWindow::glfw3NativeWindow(const std::shared_ptr<NativeGLContext>& context) {
  SEQUOIA_ASSERT_MSG(isa<glfw3NativeGLContext>(context.get()), "expected 'glfw3NativeGLContext'");
  context_ = dyn_pointer_cast<glfw3NativeGLContext>(context);

  LOG(INFO) << "Initializing glfw3 window " << this << " ...";

  SEQUOIA_ASSERT_MSG(getGLFWwindow(), "invalid context - not initialized?");

  // Query window geometry
  glfwGetWindowSize(getGLFWwindow(), &width_, &height_);
  LOG(INFO) << "Setting window geometry: " << width_ << " x " << height_;

  // Query focus status
  focused_ = glfwGetWindowAttrib(getGLFWwindow(), GLFW_FOCUSED);

  // Register the window
  glfw3NativeWindow::WindowMap.emplace(getGLFWwindow(), this);

  // Register window call-back
  glfwSetWindowSizeCallback(context_->getGLFWwindow(), CallbackResized);
  glfwSetWindowFocusCallback(context_->getGLFWwindow(), CallbackFocused);

  LOG(INFO) << "Successfully initialized glfw3 window " << this;
}

glfw3NativeWindow::~glfw3NativeWindow() { glfw3NativeWindow::WindowMap.erase(getGLFWwindow()); }

void glfw3NativeWindow::swapBuffers() { glfwSwapBuffers(getGLFWwindow()); }

bool glfw3NativeWindow::isHidden() { return glfwGetWindowAttrib(getGLFWwindow(), GLFW_VISIBLE); }

bool glfw3NativeWindow::isClosed() { return glfwWindowShouldClose(getGLFWwindow()); }

bool glfw3NativeWindow::isFullscreen() const {
  return glfwGetWindowMonitor(getGLFWwindow()) != nullptr;
}

bool glfw3NativeWindow::isFocused() const { return focused_; }

int glfw3NativeWindow::getWidth() const { return width_; }

int glfw3NativeWindow::getHeight() const { return height_; }

void glfw3NativeWindow::setGeometry(int width, int height) {
  width_ = width;
  height_ = height;
  for(NativeWindowListener* listener : getListeners<NativeWindowListener>())
    listener->nativeWindowGeometryChanged(this);
}

void glfw3NativeWindow::setFocus(bool focus) {
  focused_ = focus;
  for(NativeWindowListener* listener : getListeners<NativeWindowListener>())
    listener->nativeWindowFocusChanged(this);
}

glfw3NativeWindow* glfw3NativeWindow::getNativeWindow(GLFWwindow* window) {
  return glfw3NativeWindow::WindowMap[window];
}

//===------------------------------------------------------------------------------------------===//
//    InputSystem
//===------------------------------------------------------------------------------------------===//

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  glfw3NativeInputSystem::getInstance()->keyCallback(key, action, mods);
}

static void CallbackMouseButton(GLFWwindow* window, int button, int action, int mods) {
  glfw3NativeInputSystem::getInstance()->mouseButtonCallback(button, action, mods);
}

static void CallbackCursorPos(GLFWwindow* window, double xpos, double ypos) {
  glfw3NativeInputSystem::getInstance()->mousePositionCallback(xpos, ypos);
}

static void CallbackCursorEnter(GLFWwindow* window, int entered) {
  if(entered) {
    glfw3NativeInputSystem* inputSys = glfw3NativeInputSystem::getInstance();
    if(inputSys->getCursorMode() == RenderWindow::CursorModeKind::CK_Disabled)
      inputSys->centerCursor();
  }
}

glfw3NativeInputSystem* glfw3NativeInputSystem::InputSystemInstance = nullptr;

glfw3NativeInputSystem::glfw3NativeInputSystem(const std::shared_ptr<NativeWindow>& window,
                                               bool centerCursor)
    : prevPosX_(0), prevPosY_(0), ignoreNextMousePosEvent_(true) {

  SEQUOIA_ASSERT_MSG(!glfw3NativeInputSystem::InputSystemInstance,
                     "glfw3 input system already initialized");

  SEQUOIA_ASSERT_MSG(isa<glfw3NativeWindow>(window.get()), "expected 'glfw3NativeWindow'");
  window_ = dyn_pointer_cast<glfw3NativeWindow>(window);

  LOG(INFO) << "Registering glfw3 input system " << this << " ...";

  SEQUOIA_ASSERT_MSG(getGLFWwindow(), "invalid context - not initialized?");

  if(centerCursor)
    this->centerCursor();

  double xPos, yPos;
  glfwGetCursorPos(getGLFWwindow(), &xPos, &yPos);
  prevPosX_ = xPos;
  prevPosY_ = yPos;

  glfwSetInputMode(getGLFWwindow(), GLFW_STICKY_KEYS, 1);
  glfwSetKeyCallback(getGLFWwindow(), KeyCallback);
  glfwSetMouseButtonCallback(getGLFWwindow(), CallbackMouseButton);
  glfwSetCursorPosCallback(getGLFWwindow(), CallbackCursorPos);
  glfwSetCursorEnterCallback(getGLFWwindow(), CallbackCursorEnter);

  glfw3NativeInputSystem::InputSystemInstance = this;

  LOG(INFO) << "Successfully initialized glfw3 input system " << this << " ...";
}

glfw3NativeInputSystem::~glfw3NativeInputSystem() {
  glfw3NativeInputSystem::InputSystemInstance = nullptr;
}

void glfw3NativeInputSystem::pollEvents() { glfwPollEvents(); }

void glfw3NativeInputSystem::centerCursor() {
  int xPos = window_->getWidth() / 2;
  int yPos = window_->getHeight() / 2;
  setCursorPosition(xPos, yPos);
  prevPosX_ = xPos;
  prevPosY_ = yPos;
}

void glfw3NativeInputSystem::setCursorMode(RenderWindow::CursorModeKind mode) {
  cursorMode_ = mode;
  switch(cursorMode_) {
  case RenderWindow::CursorModeKind::CK_Disabled:
    glfwSetInputMode(getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
  case RenderWindow::CursorModeKind::CK_Hidden:
    glfwSetInputMode(getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    break;
  case RenderWindow::CursorModeKind::CK_Normal:
    glfwSetInputMode(getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  }
}

void glfw3NativeInputSystem::setCursorPosition(int xPos, int yPos) {
  glfwSetCursorPos(getGLFWwindow(), xPos, yPos);
}

void glfw3NativeInputSystem::keyCallback(int key, int action, int mods) noexcept {
  KeyboardEvent event{(KeyboardKey)key, (KeyAction)action, mods};
  for(KeyboardListener* listener : getListeners<KeyboardListener>())
    listener->keyboardEvent(event);
}

void glfw3NativeInputSystem::mouseButtonCallback(int button, int action, int mods) noexcept {
  MouseButtonEvent event{(MouseButton)button, (KeyAction)action, mods};
  for(MouseListener* listener : getListeners<MouseListener>())
    listener->mouseButtonEvent(event);
}

void glfw3NativeInputSystem::mousePositionCallback(int xPos, int yPos) noexcept {
  if(SEQUOIA_BUILTIN_UNLIKELY(ignoreNextMousePosEvent_)) {
    ignoreNextMousePosEvent_ = false;
    return;
  }

  MousePositionEvent event{xPos, yPos, xPos - prevPosX_, yPos - prevPosY_};
  for(MouseListener* listener : getListeners<MouseListener>())
    listener->mousePositionEvent(event);

  prevPosX_ = xPos;
  prevPosY_ = yPos;
}

} // render

} // namespace sequoia
