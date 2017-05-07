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
#include "sequoia/Render/Camera.h"
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
    : RenderWindow(RK_GLRenderWindow), window_(nullptr), isFullscreen_(false), windowWidth_(-1),
      windowHeight_(-1) {
  LOG(INFO) << "Initializing window " << this << " ...";

  // Set window hints
  Options& opt = Options::getSingleton();

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
    SEQUOIA_THROW(RenderSystemInitException, "GLFW failed to initialize window");

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

  LOG(INFO) << "Done initializing window";
}

GLRenderWindow::~GLRenderWindow() {
  LOG(INFO) << "Terminating window " << this << " ...";

  glfwMakeContextCurrent(window_);
  // glbinding::Binding::releaseCurrentContext();
  StaticWindowMap.erase(window_);
  glfwDestroyWindow(window_);

  LOG(INFO) << "Done terminating window";
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

// ---------------------------------------------
GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */
// ---------------------------------------------

void GLRenderWindow::update() {
  // Assert matrix mode is modelview (should be as we called the camera projection prev.)

  // ---------------------------------------------  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glTranslatef(0.0, 0.0, -1.0);
  glRotatef(60, 1.0, 0.0, 0.0);
  glRotatef(-20, 0.0, 0.0, 1.0);
  
  for (int i = 0; i < 6; i++) {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
  // ---------------------------------------------
  
}

void GLRenderWindow::init() {
  Options& opt = Options::getSingleton();
  LOG(INFO) << "Initializing OpenGL ...";

  glfwMakeContextCurrent(window_);
  glbinding::Binding::initialize(false);

  // Set debugging callbacks
  if(opt.Core.Debug) {
    using namespace glbinding;
    setCallbackMaskExcept(CallbackMask::After | CallbackMask::ParametersAndReturnValue,
                          {"glGetError", 
                           
                           //TODO: Problem is in begin/end we cannot call glGetError
                           "glBegin", "glEnd", "glVertex3f", "glNormal3fv", "glVertex3fv"});
    setAfterCallback([](const FunctionCall& call) {
      const auto error = glGetError();
      if(error != GL_NO_ERROR)
        LOG(ERROR) << "GL_ERROR: " << error << ": " << FunctionCallToString(call);
    });
  }

  LOG(INFO) << "glbinding: " << GLBINDING_VERSION;
  LOG(INFO) << "OpenGL version: " << glbinding::ContextInfo::version().toString();
  LOG(INFO) << "OpenGL vendor: " << glbinding::ContextInfo::vendor();
  LOG(INFO) << "OpenGL renderer: " << glbinding::ContextInfo::renderer();

  // Initalize OpenGL
  // ---------------------------------------------
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;
  
  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  
  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);
  // ---------------------------------------------
  
  
  // Call resize manually to set the viewport
  resizeCallback(windowWidth_, windowHeight_);

  LOG(INFO) << "Done initializing OpenGL";
}

GLFWwindow* GLRenderWindow::getGLFWwindow() { return window_; }

} // namespace render

} // namespace sequoia
