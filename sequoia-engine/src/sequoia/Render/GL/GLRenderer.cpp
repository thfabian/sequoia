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
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Math/CoordinateSystem.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/DrawCommandList.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/RenderSystem.h"
#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>
#include <glbinding/glbinding-version.h>
#include <sstream>

namespace sequoia {

namespace render {

static const char* getGLType(GLenum type) {
  switch(type) {
  case GL_DEBUG_TYPE_ERROR:
    return "ERROR";
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    return "DEPRECATED_BEHAVIOR";
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    return "UNDEFINED_BEHAVIOR";
  case GL_DEBUG_TYPE_PORTABILITY:
    return "PORTABILITY";
  case GL_DEBUG_TYPE_PERFORMANCE:
    return "PERFORMANCE";
  case GL_DEBUG_TYPE_OTHER:
    return "OTHER";
  default:
    return "UNKNOWN";
  }
}

static void GLDebugLogging(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                           const GLchar* message, const void* userParam) {
  switch(severity) {
  case GL_DEBUG_SEVERITY_LOW:
    LOG(INFO) << "GL_" << getGLType(type) << ": " << message;
    return;
  case GL_DEBUG_SEVERITY_MEDIUM:
    LOG(WARNING) << "GL_" << getGLType(type) << ": " << message;
    return;
  case GL_DEBUG_SEVERITY_HIGH:
    LOG(ERROR) << "GL_" << getGLType(type) << ": " << message;
    return;
  default:
    LOG(DEBUG) << "GL_" << getGLType(type) << ": " << message;
    return;
  }
}

static std::string functionCallToString(const glbinding::FunctionCall& call) {
  std::stringstream ss;
  ss << call.function->name()
     << core::RangeToString(", ", "(", ")")(
            call.parameters, [](glbinding::AbstractValue* value) { return value->asString(); });
  if(call.returnValue)
    ss << " -> " << call.returnValue->asString();
  return ss.str();
}

GLRenderer::GLRenderer(GLRenderWindow* window) : window_(window) {
  LOG(INFO) << "Creating OpenGL renderer " << this << " ...";

  // Bind the context to the current thread
  glfwMakeContextCurrent(window_->getGLFWwindow());

  // Load function lazily
  glbinding::Binding::initialize(false);

  // Set debugging callbacks
  if(RenderSystem::getSingleton().debugMode()) {

    // Enable error checking
    using namespace glbinding;
    setCallbackMaskExcept(CallbackMask::After | CallbackMask::ParametersAndReturnValue,
                          {"glGetError"});
    setAfterCallback([](const FunctionCall& call) {
      const auto error = glGetError();
      if(error != GL_NO_ERROR)
        LOG(ERROR) << "GL_ERROR: " << error << ": " << functionCallToString(call);
    });

    // Enable logging
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugLogging, nullptr);
  }

  LOG(INFO) << "glbinding: " << GLBINDING_VERSION;
  LOG(INFO) << "GL version: " << glbinding::ContextInfo::version().toString();
  LOG(INFO) << "GL vendor: " << glbinding::ContextInfo::vendor();
  LOG(INFO) << "GL renderer: " << glbinding::ContextInfo::renderer();

  // Enable VSync?
  if(Options::getSingleton().Render.VSync)
    glfwSwapInterval(1);

  // Register as Viewport listener
  window_->getViewport()->addListener(static_cast<ViewportListener*>(this));

  // Initialize OpenGL related managers
  stateCacheManager_ = std::make_unique<GLStateCacheManager>();
  shaderManager_ = std::make_unique<GLShaderManager>();
  programManager_ = std::make_unique<GLProgramManager>();

  LOG(INFO) << "Done creating OpenGL renderer " << this;
}

GLRenderer::~GLRenderer() {
  LOG(INFO) << "Terminating OpenGL renderer " << this << " ... ";

  glfwMakeContextCurrent(window_->getGLFWwindow());

  // Destroy all remaining shaders, programs, textures and buffers
  defaultVertexShader_.reset();
  defaultFragmentShader_.reset();
  defaultProgram_.reset();

  programManager_.reset();
  shaderManager_.reset();
  glbinding::Binding::releaseCurrentContext();

  LOG(INFO) << "Done terminating OpenGL renderer " << this << " ... ";
}

void GLRenderer::render() {
  Viewport* viewport = window_->getViewport();
  Camera* camera = viewport->getCamera();
  DrawCommandList* drawCommandList = window_->getDrawCommandList().get();

  // Compute projection view matrix
  math::mat4 matViewProj = camera->getViewProjectionMatrix();

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start rendering
  stateCacheManager_->startRendering();

  DrawCommand* drawCommand = nullptr;
  for(drawCommand = drawCommandList->start(); drawCommand != nullptr;
      drawCommand = drawCommandList->next()) {

    GLProgram* program = dyn_cast<GLProgram>(drawCommand->getProgram());

    // Compute the full model-view-projection matrix
    UniformVariable u_ModelViewProjection = matViewProj * drawCommand->getModelMatrix();
    program->setUniformVariable("u_ModelViewProjection", u_ModelViewProjection);
    
    // Update the OpenGL state-machine and draw the command
    stateCacheManager_->draw(drawCommand);
  }
}

GLShaderManager* GLRenderer::getShaderManager() { return shaderManager_.get(); }

GLProgramManager* GLRenderer::getProgramManager() { return programManager_.get(); }

GLStateCacheManager* GLRenderer::getStateCacheManager() { return stateCacheManager_.get(); }

void GLRenderer::loadDefaultShaders(const std::shared_ptr<File>& defaultVertexShaderFile,
                                    const std::shared_ptr<File>& defaultFragmentShaderFile) {
  auto& rsys = RenderSystem::getSingleton();

  defaultVertexShader_ = rsys.loadShader(window_, Shader::ST_Vertex, defaultVertexShaderFile);
  defaultFragmentShader_ = rsys.loadShader(window_, Shader::ST_Fragment, defaultFragmentShaderFile);
  defaultProgram_ = rsys.createProgram(window_, {defaultVertexShader_, defaultFragmentShader_});
}

const std::shared_ptr<Shader>& GLRenderer::getDefaultVertexShader() const {
  return defaultVertexShader_;
}

const std::shared_ptr<Shader>& GLRenderer::getDefaultFragmentShader() const {
  return defaultFragmentShader_;
}

const std::shared_ptr<Program>& GLRenderer::getDefaultProgram() const { return defaultProgram_; }

void GLRenderer::viewportGeometryChanged(Viewport* viewport) {
  glViewport(viewport->getX(), viewport->getY(), viewport->getWidth(), viewport->getHeight());
}

} // namespace render

} // namespace sequoia
