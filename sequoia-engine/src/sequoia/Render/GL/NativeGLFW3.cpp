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

  LOG(INFO) << "Sucessfuly initialized glfw3 OpenGL context " << this << " ...";
}

void glfw3NativeGLContext::init(const std::shared_ptr<NativeGLContext>& context) {}

glfw3NativeGLContext* glfw3NativeGLContext::getNativeGLContext(GLFWwindow* window) {
  return ContextMap[window];
}

//===------------------------------------------------------------------------------------------===//
//    Window
//===------------------------------------------------------------------------------------------===//

static void CallbackResize(GLFWwindow* window, int width, int height) {
  glfw3NativeWindow::getNativeWindow(window)->resize(width, height);
}

std::unordered_map<GLFWwindow*, glfw3NativeWindow*> glfw3NativeWindow::WindowMap;

glfw3NativeWindow::glfw3NativeWindow(const std::shared_ptr<NativeGLContext>& context)
    : NativeWindow(context) {
  SEQUOIA_ASSERT_MSG(isa<glfw3NativeGLContext>(context.get()), "expected 'glfw3NativeGLContext'");
  context_ = dyn_pointer_cast<glfw3NativeGLContext>(context);

  LOG(INFO) << "Initializing glfw3 window " << this << " ...";

  SEQUOIA_ASSERT_MSG(context_->getGLFWwindow(), "invalid context - not initialized?");

  // Query width and height
  int width, height;
  glfwGetWindowSize(window_, &width, &height);
  LOG(INFO) << "Setting window geometry: " << width << " x " << height;

  // Register the window
  glfw3NativeWindow::WindowMap.emplace(context_->getGLFWwindow(), this);

  // Register window call-back
  glfwSetWindowSizeCallback(context_->getGLFWwindow(), CallbackResize);

  LOG(INFO) << "Sucessfuly initialized glfw3 window " << this << " ...";
}

glfw3NativeWindow::~glfw3NativeWindow() { glfw3NativeWindow::WindowMap.erase(this); }

//void glfw3NativeWindow::swapBuffers()
//{
  
//}

//bool glfw3NativeWindow::isHidden()
//{
  
//}

//bool glfw3NativeWindow::isClosed()
//{
  
//}

//bool glfw3NativeWindow::isFullscreen() const
//{
  
//}

//int glfw3NativeWindow::getWidth() const
//{
  
//}

//int glfw3NativeWindow::getHeight() const
//{
  
//}

glfw3NativeWindow* glfw3NativeWindow::getNativeWindow(GLFWwindow* window) {
  return glfw3NativeWindow::WindowMap[window];
}

//===------------------------------------------------------------------------------------------===//
//    InputSystem
//===------------------------------------------------------------------------------------------===//

//  LOG(INFO) << "Registering IO callbacks ...";
//  inputSystem_ = std::make_unique<GLInputSystem>(this, true);

//  glfwSetInputMode(window_, GLFW_STICKY_KEYS, 1);
//  glfwSetKeyCallback(window_, GLRenderWindow::keyCallbackDispatch);
//  glfwSetMouseButtonCallback(window_, GLRenderWindow::mouseButtonCallbackDispatch);
//  glfwSetCursorPosCallback(window_, GLRenderWindow::mousePositionCallbackDispatch);
//  glfwSetCursorEnterCallback(window_, GLRenderWindow::mouseEnterCallbackDispatch);

//  // Register the window as an input event listener
//  getGLRenderSystem().addListener<InputEventListener>(this);

//  LOG(INFO) << "Done registering IO callbacks";

//  LOG(INFO) << "Done initializing OpenGL window " << this;


} // render

} // namespace sequoia
