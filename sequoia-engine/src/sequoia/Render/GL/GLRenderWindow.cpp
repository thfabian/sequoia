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
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>
#include <glbinding/glbinding-version.h>
#include <unordered_map>

namespace sequoia {

namespace render {

static std::string FunctionCallToString(const glbinding::FunctionCall& call) {
  std::stringstream ss;
  ss << call.function->name()
     << core::RangeToString(", ", "(", ")")(
            call.parameters, [](glbinding::AbstractValue* value) { return value->asString(); });
  if(call.returnValue)
    ss << " -> " << call.returnValue->asString();
  return ss.str();
}

std::unordered_map<GLFWwindow*, GLRenderWindow*> GLRenderWindow::StaticWindowMap;

GLRenderWindow::GLRenderWindow(const std::string& title)
    : window_(nullptr), isFullscreen_(false), width_(-1), height_(-1) {
  LOG(INFO) << "Initializing window " << this << " ...";

  // Set window hints
  Options& opt = Options::getSingleton();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opt.Render.GLMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opt.Render.GLMinorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_SAMPLES, opt.Render.FSAA);
  LOG(INFO) << "Using FSAA: " << opt.Render.FSAA;

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
      window_ = glfwCreateWindow(1024, 768, title.c_str(), nullptr, nullptr);
    } else if(opt.Render.WindowMode == "windowed-fullscreen") {
      window_ = glfwCreateWindow(mode->width, mode->height, title.c_str(), nullptr, nullptr);
    } else
      sequoia_unreachable("invalid window-mode");
  }
  LOG(INFO) << "Using window mode: " << opt.Render.WindowMode;

  if(!window_)
    SEQUOIA_THROW(RenderSystemInitException,
                  "failed to initialize window, requested OpenGL (%i. %i)",
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
  glfwGetWindowSize(window_, &width_, &height_);
  LOG(INFO) << "Using window geometry: " << width_ << " x " << height_;
  
  // Register the window "globally"
  StaticWindowMap.emplace(window_, this);
  
  // Register window call-back
  glfwSetWindowSizeCallback(window_, GLRenderWindow::resizeCallbackDispatch);

  LOG(INFO) << "Done initializing window";
}

GLRenderWindow::~GLRenderWindow() {
  LOG(INFO) << "Terminating window " << this << " ...";

  glfwMakeContextCurrent(window_);
  glbinding::Binding::releaseCurrentContext();
  StaticWindowMap.erase(window_);
  glfwDestroyWindow(window_);

  LOG(INFO) << "Done terminating window";
}

bool GLRenderWindow::isClosed() { return glfwWindowShouldClose(window_); }

bool GLRenderWindow::isFullscreen() const { return isFullscreen_; }

void GLRenderWindow::resizeCallback(int width, int height) {
  width_ = width;
  height_ = height;
  
  // Reset the current viewport
  glViewport(0, 0, width_, height_);
  
  // Select the projection matrix
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


  
  // Calcualte aspect ratio
  glMatrixMode(GL_MODELVIEW);  
  glLoadIdentity();  
}

void GLRenderWindow::resizeCallbackDispatch(GLFWwindow* window, int width, int height) {
  GLRenderWindow::StaticWindowMap[window]->resizeCallback(width, height);
}

int GLRenderWindow::getWidth() const { return width_; }

int GLRenderWindow::getHeight() const { return height_; }

void GLRenderWindow::swapBuffers() { glfwSwapBuffers(window_); }

void GLRenderWindow::renderOneFrame() {
  // Clear screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset the current modelview matrix
  glLoadIdentity();
  
//  glColor3f(1.0,1.0,1.0);
//  glBegin(GL_QUADS);                                  
//  glVertex3f(-0.5,-0.5,-0.5);
//  glVertex3f(0.5,-0.5,-0.5);
//  glVertex3f(0.5,0.5,-0.5);
//  glVertex3f(-0.5,0.5,-0.5);
//  glEnd();
}

GLFWwindow* GLRenderWindow::getGLFWwindow() { return window_; }

void GLRenderWindow::init() {
  Options& opt = Options::getSingleton();
  LOG(INFO) << "Initializing up OpenGL ...";

  // Initialize glbinding
  glfwMakeContextCurrent(window_);
  glbinding::Binding::initialize();
  LOG(INFO) << "glbinding: " << GLBINDING_VERSION;

  LOG(INFO) << "OpenGL version: " << glbinding::ContextInfo::version().toString();
  LOG(INFO) << "OpenGL vendor: " << glbinding::ContextInfo::vendor();
  LOG(INFO) << "OpenGL renderer: " << glbinding::ContextInfo::renderer();

  // Set debugging callbacks
  if(opt.Core.Debug) {
    using namespace glbinding;
    setCallbackMaskExcept(CallbackMask::After | CallbackMask::ParametersAndReturnValue,
                          {"glGetError"});
    setAfterCallback([](const FunctionCall& call) {
      const auto error = glGetError();
      if(error != GL_NO_ERROR)
        LOG(ERROR) << "GL_ERROR: " << error << ": " << FunctionCallToString(call);
    });
  }

  // Initalize OpenGL
  glShadeModel(GL_SMOOTH);							 // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	 // Black Background
	glClearDepth(1.0f);									   // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							 // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								 // The Type Of Depth Testing To Do
  
  resizeCallback(width_, height_);
  
  LOG(INFO) << "Done initializing OpenGL";  
}

} // namespace render

} // namespace sequoia
