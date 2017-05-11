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
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>
#include <glbinding/glbinding-version.h>
#include <iostream>
#include <sstream>

namespace sequoia {

namespace render {

static std::string functionCallToString(const glbinding::FunctionCall& call) {
  std::stringstream ss;
  ss << call.function->name()
     << core::RangeToString(", ", "(", ")")(
            call.parameters, [](glbinding::AbstractValue* value) { return value->asString(); });
  if(call.returnValue)
    ss << " -> " << call.returnValue->asString();
  return ss.str();
}

GLRenderer::GLRenderer(GLRenderWindow* target) : target_(target) {
  Options& opt = Options::getSingleton();
  LOG(INFO) << "Creating OpenGL Renderer " << this << " ...";

  // Bind the context to the current thread
  glfwMakeContextCurrent(target_->getGLFWwindow());

  // Load function lazily
  glbinding::Binding::initialize(false);

  // Set debugging callbacks
  if(opt.Core.Debug) {
    using namespace glbinding;
    setCallbackMaskExcept(CallbackMask::After | CallbackMask::ParametersAndReturnValue,
                          {"glGetError"});
    setAfterCallback([](const FunctionCall& call) {
      const auto error = glGetError();
      if(error != GL_NO_ERROR)
        LOG(ERROR) << "GL_ERROR: " << error << ": " << functionCallToString(call);
    });
  }

  LOG(INFO) << "glbinding: " << GLBINDING_VERSION;
  LOG(INFO) << "OpenGL version: " << glbinding::ContextInfo::version().toString();
  LOG(INFO) << "OpenGL vendor: " << glbinding::ContextInfo::vendor();
  LOG(INFO) << "OpenGL renderer: " << glbinding::ContextInfo::renderer();

  // Initialize ShaderManager
  shaderManager_ = std::make_unique<GLShaderManager>();

  LOG(INFO) << "Done creating OpenGL renderer " << this;
}

GLRenderer::~GLRenderer() {
  LOG(INFO) << "Terminating OpenGL Renderer " << this << " ... ";

  glfwMakeContextCurrent(target_->getGLFWwindow());
  glbinding::Binding::releaseCurrentContext();

  LOG(INFO) << "Done terminating OpenGL renderer " << this << " ... ";
}

void GLRenderer::render() {
  Viewport* viewport = target_->getViewport();
  Camera* camera = viewport->getCamera();

  // Compute the projction matrix
  glm::mat4 matProj =
      glm::perspective(glm::radians(camera->getFieldOfViewY()), camera->getAspectRatio(),
                       camera->getZNearClipping(), camera->getZFarClipping());

  // Compute camera view matrix
  glm::mat4 matView = glm::lookAt(camera->getEye(), camera->getCenter(), camera->getUp());

  // Precompute view projection matrix
  glm::mat4 matViewProj = matProj * matView;

  glm::mat4 matModel = glm::mat4(1.0f);

  // Compute the full model view projection matrix
  glm::mat4 matModelViewProjection = matViewProj * matModel;
}

GLShaderManager* GLRenderer::getShaderManager() { return shaderManager_.get(); }

} // namespace render

} // namespace sequoia
