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

#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderSystem.h"

namespace sequoia {

namespace render {

static void GLFWErrorCallbackSoft(int error, const char* description) {
  LOG(ERROR) << "GLFW error: " << description;
}

static void GLFWErrorCallbackHard(int error, const char* description) {
  GLFWErrorCallbackSoft(error, description);
  SEQUOIA_THROW(RenderSystemInitException, description);
}

GLRenderSystem::GLRenderSystem() {
  LOG(INFO) << "Initializing GLRenderSystem ...";
  glfwSetErrorCallback(GLFWErrorCallbackHard);

  glfwInit();
  LOG(INFO) << "GLFW: " << glfwGetVersionString();

  glfwSetErrorCallback(GLFWErrorCallbackSoft);
  LOG(INFO) << "Done initializing GLRenderSystem";
}

GLRenderSystem::~GLRenderSystem() {
  LOG(INFO) << "Terminating GLRenderSystem ...";
  windowMap_.clear();
  glfwTerminate();
  LOG(INFO) << "Done terminating GLRenderSystem";
}

int GLRenderSystem::createWindow(const std::string& title) {
  auto res = windowMap_.emplace(windowMap_.size(), std::make_shared<GLRenderWindow>(title));
  SEQUOIA_ASSERT_MSG(res.second, "failed to create window");
  return res.first->first; // WindowID
}

RenderWindow* GLRenderSystem::getWindow(int windowID) {
  return static_cast<RenderWindow*>(windowMap_[windowID].get());
}

void GLRenderSystem::pollEvents() { glfwPollEvents(); }

} // namespace render

} // namespace sequoia
