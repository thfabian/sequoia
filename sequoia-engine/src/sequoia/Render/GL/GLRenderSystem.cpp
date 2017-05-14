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

#include "sequoia/Core/Casting.h"
#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"

namespace sequoia {

namespace render {

static void GLFWErrorCallbackSoft(int error, const char* description) {
  LOG(ERROR) << "GLFW error: " << description;
}

static void GLFWErrorCallbackHard(int error, const char* description) {
  GLFWErrorCallbackSoft(error, description);
  SEQUOIA_THROW(RenderSystemException, description);
}

GLRenderSystem::GLRenderSystem() : RenderSystem(RK_OpenGL) {
  LOG(INFO) << "Initializing GLRenderSystem ...";
  glfwSetErrorCallback(GLFWErrorCallbackHard);

  glfwInit();
  LOG(INFO) << "GLFW: " << glfwGetVersionString();

  glfwSetErrorCallback(GLFWErrorCallbackSoft);
  LOG(INFO) << "Done initializing GLRenderSystem";
}

GLRenderSystem::~GLRenderSystem() {
  LOG(INFO) << "Terminating GLRenderSystem ...";
  renderTargets_.clear();
  glfwTerminate();
  LOG(INFO) << "Done terminating GLRenderSystem";
}

RenderWindow* GLRenderSystem::createWindow(const RenderWindow::WindowHint& hints) {
  renderTargets_.emplace_back(std::make_shared<GLRenderWindow>(this, hints));
  return static_cast<RenderWindow*>(renderTargets_.back().get());
}

void GLRenderSystem::destroyTarget(RenderTarget* target) {
  rendererMap_.erase(target);
  for(auto it = renderTargets_.begin(); it != renderTargets_.end();)
    if(it->get() == target) {
      it = renderTargets_.erase(it);
    } else
      ++it;
}

void GLRenderSystem::pollEvents() { glfwPollEvents(); }

void GLRenderSystem::renderOneFrame() {
  std::for_each(renderTargets_.begin(), renderTargets_.end(), [](const auto& target) {
    if(target->isActive())
      target->update();
  });
}

void GLRenderSystem::swapBuffers() {
  std::for_each(renderTargets_.begin(), renderTargets_.end(), [](const auto& target) {
    if(target->isActive())
      target->swapBuffers();
  });
}

Shader* GLRenderSystem::loadShader(RenderTarget* target, Shader::ShaderType type,
                                   const platform::String& path) {
  return rendererMap_[target]->getShaderManager()->create(type, path);
}

void GLRenderSystem::destroyShader(RenderTarget* target, Shader* shader) {
  rendererMap_[target]->getShaderManager()->destroy(dyn_cast<GLShader>(shader));
}

Program* GLRenderSystem::createProgram(RenderTarget* target, const std::set<Shader*>& shaders) {
  return nullptr;
}

void GLRenderSystem::registerRenderer(RenderTarget* target, GLRenderer* renderer) {
  rendererMap_[target] = renderer;
}

GLRenderer* GLRenderSystem::getRenderer(RenderTarget* target) const {
  return rendererMap_.find(target)->second;
}

} // namespace render

} // namespace sequoia
