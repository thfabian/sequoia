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

#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Core/Casting.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Math/CoordinateSystem.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/DrawCommandList.h"
#include "sequoia-engine/Render/GL/GLExtensionManager.h"
#include "sequoia-engine/Render/GL/GLProgramManager.h"
#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/GL/GLRenderWindow.h"
#include "sequoia-engine/Render/GL/GLRenderer.h"
#include "sequoia-engine/Render/GL/GLShaderManager.h"
#include "sequoia-engine/Render/GL/GLStateCacheManager.h"
#include "sequoia-engine/Render/GL/GLTextureManager.h"
#include "sequoia-engine/Render/GL/Native.h"
#include "sequoia-engine/Render/RenderSystem.h"
#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Meta.h>
#include <glbinding/Version.h>
#include <glbinding/glbinding-version.h>
#include <sstream>
#include <unordered_set>

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
  window_->getContext()->makeCurrent();

  // Load functions lazily
  glbinding::Binding::initialize(false);

  // Set debugging callbacks
  if(getGLRenderSystem().getOptions().Core.Debug) {

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
  if(getGLRenderSystem().getOptions().Render.VSync)
    window_->getContext()->enableVSync();

  // Register as Viewport listener
  window_->getViewport()->addListener<ViewportListener>(this);

  // Initialize OpenGL related managers
  stateCacheManager_ = std::make_unique<GLStateCacheManager>();
  addListener<FrameListener>(stateCacheManager_.get());
  shaderManager_ = std::make_unique<GLShaderManager>();
  programManager_ = std::make_unique<GLProgramManager>();
  textureManager_ = std::make_unique<GLTextureManager>();
  extensionManager_ = std::make_unique<GLExtensionManager>();

  LOG(INFO) << "Successfully created OpenGL renderer " << this;
}

GLRenderer::~GLRenderer() {
  LOG(INFO) << "Terminating OpenGL renderer " << this << " ... ";

  window_->getContext()->makeCurrent();

  // Destroy all remaining shaders, programs, textures and buffers
  defaultVertexShader_.reset();
  defaultFragmentShader_.reset();
  defaultProgram_.reset();

  programManager_.reset();
  shaderManager_.reset();
  textureManager_.reset();

  stateCacheManager_.reset();

  glbinding::Binding::releaseCurrentContext();

  LOG(INFO) << "Done terminating OpenGL renderer " << this << " ... ";
}

void GLRenderer::render(RenderCommand* command) {
  RenderTarget* target = command->getRenderTarget();
  SEQUOIA_ASSERT_MSG(target, "no RenderTarget set");

  // Bind the FrameBuffer - if no FBO is attached, we render to the main-screen
  if(target->hasFrameBuffer())
    stateCacheManager_->bindFrameBufferObject(target->getFrameBuffer().get());

  // Inform everyone that we start rendering a frame
  for(FrameListener* listener : getListeners<FrameListener>())
    listener->frameListenerRenderingBegin(command);

  Viewport* viewport = target->getViewport();
  SEQUOIA_ASSERT_MSG(viewport, "no Viewport set");

  Camera* camera = viewport->getCamera();
  SEQUOIA_ASSERT_MSG(camera, "no Camera set");

  DrawCommandList* drawCommandList = command->getDrawCommandList();
  SEQUOIA_ASSERT_MSG(drawCommandList, "no DrawCommandList set");

  // Set the viewport
  stateCacheManager_->setViewport(viewport);

  // Compute projection-view matrix
  math::mat4 matVP = camera->getViewProjectionMatrix();
  UniformVariable u_matP = camera->getProjectionMatrix();

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Extract a list of all referenced, valid programs
  std::unordered_set<Program*> programs;
  for(DrawCommand* drawCommand = drawCommandList->start(); drawCommand != nullptr;
      drawCommand = drawCommandList->next()) {
    programs.insert(drawCommand->getProgram());
  }

  // Set the uniform variables which are persistent accross the DrawCommands for each program
  command->forEachUniformVariable(
      programs, [this](Program* program, const std::string& name, const UniformVariable& variable) {
        stateCacheManager_->setUniformVariable(program, name, variable);
      });

  for(DrawCommand* drawCommand = drawCommandList->start(); drawCommand != nullptr;
      drawCommand = drawCommandList->next()) {
    GLProgram* program = dyn_cast<GLProgram>(drawCommand->getProgram());
    if(!program->isValid())
      continue;

    // Compute the transformation matrices (camera to world space)
    UniformVariable u_matM = drawCommand->getModelMatrix();
    UniformVariable u_matMVP = matVP * drawCommand->getModelMatrix();
    program->setUniformVariable("u_matMVP", u_matMVP);
    program->setUniformVariable("u_matM", u_matM);
    program->setUniformVariable("u_matP", u_matP);

    // Update the OpenGL state-machine and draw the command
    stateCacheManager_->draw(drawCommand);
  }

  // Inform everyone that we finished rendering the frame
  for(FrameListener* listener : getListeners<FrameListener>())
    listener->frameListenerRenderingEnd(command);

  // Unbind the FBO
  if(target->hasFrameBuffer())
    stateCacheManager_->unbindFrameBufferObject();
}

GLShaderManager* GLRenderer::getShaderManager() { return shaderManager_.get(); }

GLProgramManager* GLRenderer::getProgramManager() { return programManager_.get(); }

GLTextureManager* GLRenderer::getTextureManager() { return textureManager_.get(); }

GLStateCacheManager* GLRenderer::getStateCacheManager() { return stateCacheManager_.get(); }

void GLRenderer::loadDefaultShaders(const std::shared_ptr<File>& defaultVertexShaderFile,
                                    const std::shared_ptr<File>& defaultFragmentShaderFile) {
  auto& rsys = RenderSystem::getSingleton();

  defaultVertexShader_ = rsys.createShader(Shader::ST_Vertex, defaultVertexShaderFile);
  defaultFragmentShader_ = rsys.createShader(Shader::ST_Fragment, defaultFragmentShaderFile);
  defaultProgram_ = rsys.createProgram({defaultVertexShader_, defaultFragmentShader_});
}

const std::shared_ptr<Shader>& GLRenderer::getDefaultVertexShader() const {
  return defaultVertexShader_;
}

const std::shared_ptr<Shader>& GLRenderer::getDefaultFragmentShader() const {
  return defaultFragmentShader_;
}

const std::shared_ptr<Program>& GLRenderer::getDefaultProgram() const { return defaultProgram_; }

void GLRenderer::viewportGeometryChanged(Viewport* viewport) {
  stateCacheManager_->setViewport(viewport);
}

bool GLRenderer::isExtensionSupported(GLextension extension) noexcept {
  return extensionManager_->isSupported(extension);
}

GLRenderer& getGLRenderer() noexcept { return *getGLRenderSystem().getRenderer(); }

GLRenderer* getGLRendererPtr() noexcept { return getGLRenderSystem().getRenderer(); }

} // namespace render

} // namespace sequoia