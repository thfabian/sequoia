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
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderSystem.h"

namespace sequoia {

namespace render {

/// @brief Abort if an error occured
static void GLFWErrorCallback(int error, const char* description) {
  // TODO: log error
  ErrorHandler::getSingleton().fatal(description);
}

GLRenderSystem::GLRenderSystem() {
  glfwSetErrorCallback(GLFWErrorCallback);

  // Initialize GLFW
  glfwInit();

  // TOOD: log glfw version
  // glfwGetVersionString();
}

GLRenderSystem::~GLRenderSystem() { glfwTerminate(); }

} // namespace render

} // namespace sequoia
