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
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

namespace sequoia {

namespace render {

GLRenderWindow::GLRenderWindow(int width, int height, const std::string& title) {
  LOG(INFO) << "Initializing window " << this << " ...";

  // Create a new OpenGL context
  Options& opt = Options::getSingleton();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opt.Render.GLMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opt.Render.GLMinorVersion);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  // Initialize glbinding
  glfwMakeContextCurrent(window_);
  glbinding::Binding::initialize();

  LOG(INFO) << "OpenGL version: " << glbinding::ContextInfo::version().toString();
  LOG(INFO) << "OpenGL vendor: " << glbinding::ContextInfo::vendor();
  LOG(INFO) << "OpenGL renderer: " << glbinding::ContextInfo::renderer();

  LOG(INFO) << "Done initializing window";
}

GLRenderWindow::~GLRenderWindow() {
  LOG(INFO) << "Terminating window " << this << " ...";

  glfwMakeContextCurrent(window_);
  glbinding::Binding::releaseCurrentContext();
  glfwDestroyWindow(window_);

  LOG(INFO) << "Done terminating window";
}

bool GLRenderWindow::isOpen() { return !glfwWindowShouldClose(window_); }

GLFWwindow* GLRenderWindow::getGLFWwindow() { return window_; }

} // namespace render

} // namespace sequoia
