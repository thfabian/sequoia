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

#include "sequoia-engine/Core/Casting.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Options.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Math/CoordinateSystem.h"
#include "sequoia-engine/Render/Camera.h"
#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Render/GL/GLExtensionManager.h"
#include "sequoia-engine/Render/GL/GLProgram.h"
#include "sequoia-engine/Render/GL/GLProgramManager.h"
#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/GL/GLRenderWindow.h"
#include "sequoia-engine/Render/GL/GLRenderer.h"
#include "sequoia-engine/Render/GL/GLShaderManager.h"
#include "sequoia-engine/Render/GL/GLTextureManager.h"
#include "sequoia-engine/Render/GL/GLVertexData.h"
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

namespace {

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
    Log::trace("GL_{}: {}", getGLType(type), message);
    return;
  case GL_DEBUG_SEVERITY_MEDIUM:
    Log::warn("GL_{}: {}", getGLType(type), message);
    return;
  case GL_DEBUG_SEVERITY_HIGH:
    Log::error("GL_{}: {}", getGLType(type), message);
    return;
  default:
    Log::trace("GL_{}: {}", getGLType(type), message);
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

/// @brief Enable/Disable the capability `cap`
static void setCapability(bool enable, GLenum cap) noexcept {
  if(enable)
    glEnable(cap);
  else
    glDisable(cap);
}

} // anonymous namespace

bool GLRenderer::DepthTestChanged(bool DepthTest) {
  setCapability(DepthTest, GL_DEPTH_TEST);
  return true;
}

bool GLRenderer::DepthFuncChanged(RenderPipeline::DepthFuncKind DepthFunc) {
  switch(DepthFunc) {
  case RenderPipeline::DepthFuncKind::DF_Never:
    glDepthFunc(GL_NEVER);
    break;
  case RenderPipeline::DepthFuncKind::DF_Less:
    glDepthFunc(GL_LESS);
    break;
  case RenderPipeline::DepthFuncKind::DF_Equal:
    glDepthFunc(GL_EQUAL);
    break;
  case RenderPipeline::DepthFuncKind::DF_LessEqual:
    glDepthFunc(GL_LEQUAL);
    break;
  case RenderPipeline::DepthFuncKind::DF_Greater:
    glDepthFunc(GL_GREATER);
    break;
  case RenderPipeline::DepthFuncKind::DF_NotEqual:
    glDepthFunc(GL_NOTEQUAL);
    break;
  case RenderPipeline::DepthFuncKind::DF_GreaterEqual:
    glDepthFunc(GL_GEQUAL);
    break;
  case RenderPipeline::DepthFuncKind::DF_Always:
    glDepthFunc(GL_ALWAYS);
    break;
  default:
    sequoia_unreachable("invalid DepthFuncKind");
  }
  return true;
}

bool GLRenderer::ProgramChanged(Program* program) {
  SEQUOIA_ASSERT(program);

  if(!program->isValid())
    return false;
  
  GLProgram* glprogram = core::dyn_cast<GLProgram>(program);
  glprogram->bind();
  return true;
}

bool GLRenderer::VertexDataChanged(VertexData* data) {
  SEQUOIA_ASSERT(data);

  GLVertexData* gldata = core::dyn_cast<GLVertexData>(data);
  gldata->bind();
  return true;
}

bool GLRenderer::TextureChanged(int textureUnit, Texture* texture, bool enable) {
  SEQUOIA_ASSERT(texture);

  GLTexture* gltexture = core::dyn_cast<GLTexture>(texture);

  // Bind texture
  setActiveTextureUnit(textureUnit);

  if(enable) {
    if(!gltexture->isValid())
      return false;

    gltexture->bind();

    // Bind assoicated sampler if we already have a program
    Program* program = pipeline_.Program;
    if(program) {
      GLProgram* glprogram = core::dyn_cast<GLProgram>(program);
      const std::string& name = glprogram->getTextureSampler(textureUnit);
      setUniformVariable(glprogram, name, UniformVariable(textureUnit));
    }
  } else {
    core::dyn_cast<GLTexture>(texture)->unbind();
  }

  return true;
}

bool GLRenderer::UniformVariableChanged(Program* program, const std::string& name,
                                        const UniformVariable& value) {
  SEQUOIA_ASSERT(program);

  if(!program->isValid())
    return false;

  GLProgram* glprogram = core::dyn_cast<GLProgram>(program);
  glprogram->setUniformVariable(name, value);
  return true;
}

bool GLRenderer::ViewportChanged(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
  return true;
}

bool GLRenderer::clearRenderBuffers(
    const std::set<RenderBuffer::RenderBufferKind>& buffersToClear) {
  ClearBufferMask mask = ClearBufferMask::GL_NONE_BIT;
  for(auto buffer : buffersToClear) {
    switch(buffer) {
    case RenderBuffer::RK_Color:
      mask |= GL_COLOR_BUFFER_BIT;
      break;
    case RenderBuffer::RK_Depth:
      mask |= GL_DEPTH_BUFFER_BIT;
      break;
    case RenderBuffer::RK_Stencil:
      mask |= GL_STENCIL_BUFFER_BIT;
      break;
    default:
      sequoia_unreachable("invalid RenderBuffer::RenderBufferKind");
    }
  }
  glClear(mask);
  return true;
}

bool GLRenderer::draw(const DrawCommand& drawCommand) {
  SEQUOIA_ASSERT(drawCommand.getVertexData() == vertexData_);

  // Check all uniform variables are set
  core::dyn_cast<GLProgram>(pipeline_.Program)->checkUniformVariables();

  // Draw the vertex-data
  core::dyn_cast<GLVertexData>(vertexData_)->draw();
  return true;
}

std::pair<std::string, std::string> GLRenderer::toStringImpl() const {
  return std::make_pair("GLRenderer", core::format("{}"
                                                   "activeTextureUnit = {},\n"
                                                   "pixelFormat = {}\n",
                                                   Base::toStringImpl().second, activeTextureUnit_,
                                                   pixelFormat_.toString()));
}

GLRenderer::GLRenderer(GLRenderWindow* window) : window_(window) {
  Log::info("Creating OpenGL renderer {} ...", core::ptrToStr(this));

  // Bind the context to the current thread
  window_->getContext()->makeCurrent();

  // Load functions lazily
  glbinding::Binding::initialize(false);

  // Set debugging callbacks
  if(getGLRenderSystem().getOptions().getBool("Render.TraceAPI")) {

    // Enable error checking
    using namespace glbinding;
    setCallbackMaskExcept(CallbackMask::After | CallbackMask::ParametersAndReturnValue,
                          {"glGetError"});
    setAfterCallback([](const FunctionCall& call) {
      const auto error = glGetError();
      if(error != GL_NO_ERROR)
        Log::error("GL_ERROR: {}: {}", error, functionCallToString(call));
    });

    // Enable logging
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugLogging, nullptr);
  }

  Log::info("glbinding: {}", GLBINDING_VERSION);
  Log::info("GL version: {}", glbinding::ContextInfo::version().toString());
  Log::info("GL vendor: {}", glbinding::ContextInfo::vendor());
  Log::info("GL renderer: {}", glbinding::ContextInfo::renderer());

  // Enable VSync?
  if(getGLRenderSystem().getOptions().get<bool>("Render.VSync"))
    window_->getContext()->enableVSync();

  // Register as Viewport listener of the main-window
  window_->getViewport()->addListener<ViewportListener>(this);

  // Initialize OpenGL related managers
  shaderManager_ = std::make_unique<GLShaderManager>();
  programManager_ = std::make_unique<GLProgramManager>();
  textureManager_ = std::make_unique<GLTextureManager>();
  extensionManager_ = std::make_unique<GLExtensionManager>();
  
  // Query the default pixel format
  auto getAndSetPixelFormat = [this](GLenum param) {
    int value = get<int>(param);
    defaultPixelFormat_.set(param, value);
  };
  getAndSetPixelFormat(GL_UNPACK_ALIGNMENT);
  getAndSetPixelFormat(GL_UNPACK_ROW_LENGTH);
  
  // Reset internal state
  activeTextureUnit_ = -1;
  setPixelFormat(defaultPixelFormat_, true);  

  Log::info("Successfully created OpenGL renderer {}", core::ptrToStr(this));
}

void GLRenderer::reset() {
  Base::reset();
  activeTextureUnit_ = -1;
  setPixelFormat(defaultPixelFormat_, true);
}

GLRenderer::~GLRenderer() {
  Log::info("Terminating OpenGL renderer {} ... ", core::ptrToStr(this));

  window_->getContext()->makeCurrent();

  programManager_.reset();
  shaderManager_.reset();
  textureManager_.reset();
  glbinding::Binding::releaseCurrentContext();

  Log::info("Done terminating OpenGL renderer {} ... ", core::ptrToStr(this));
}

GLShaderManager* GLRenderer::getShaderManager() { return shaderManager_.get(); }

GLProgramManager* GLRenderer::getProgramManager() { return programManager_.get(); }

GLTextureManager* GLRenderer::getTextureManager() { return textureManager_.get(); }

bool GLRenderer::setTexture(int textureUnit, Texture* texture) {
  return setTextures(std::unordered_map<int, Texture*>{{textureUnit, texture}});
}

bool GLRenderer::unsetTextures() { return setTextures(std::unordered_map<int, Texture*>{}); }

void GLRenderer::setPixelFormat(const GLPixelFormat& format, bool force) {
  for(const auto& formatPair : format.format_) {
    GLenum param = formatPair.first;
    int value = formatPair.second;

    auto it = pixelFormat_.format_.find(param);
    if(force || (it == pixelFormat_.format_.end() || it->second != value)) {
      glPixelStorei(formatPair.first, formatPair.second);
      pixelFormat_.set(param, value);
    }
  }
}

void GLRenderer::resetPixelFormat() { setPixelFormat(defaultPixelFormat_, true); }

void GLRenderer::viewportGeometryChanged(Viewport* viewport) {
  setViewport(viewport);
}

bool GLRenderer::isExtensionSupported(GLextension extension) noexcept {
  return extensionManager_->isSupported(extension);
}

void GLRenderer::setActiveTextureUnit(int textureUnit) noexcept {
  if(activeTextureUnit_ != textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    activeTextureUnit_ = textureUnit;
  }
}

bool GLRenderer::getImpl(GLenum param, bool) const noexcept {
  GLboolean value;
  glGetBooleanv(param, &value);
  return static_cast<bool>(value);
}

int GLRenderer::getImpl(GLenum param, int) const noexcept {
  int value;
  glGetIntegerv(param, &value);
  return value;
}

float GLRenderer::getImpl(GLenum param, float) const noexcept {
  float value;
  glGetFloatv(param, &value);
  return value;
}

double GLRenderer::getImpl(GLenum param, double) const noexcept {
  double value;
  glGetDoublev(param, &value);
  return value;
}

GLRenderer& getGLRenderer() noexcept { return *getGLRenderSystem().getGLRenderer(); }

GLRenderer* getGLRendererPtr() noexcept { return getGLRenderSystem().getGLRenderer(); }

//void GLRenderer::render(RenderCommand* command) {
//  RenderTarget* target = command->getRenderTarget();
//  SEQUOIA_ASSERT_MSG(target, "no RenderTarget set");

//  // Bind the FrameBuffer - if no FBO is attached, we render to the main-screen
//  if(target->hasFrameBuffer())
//    stateCacheManager_->bindFrameBufferObject(target->getFrameBuffer().get());

//  // Inform everyone that we start rendering a frame
//  for(FrameListener* listener : getListeners<FrameListener>())
//    listener->frameListenerRenderingBegin(command);

//  Viewport* viewport = target->getViewport();
//  SEQUOIA_ASSERT_MSG(viewport, "no Viewport set");

//  Camera* camera = viewport->getCamera();
//  SEQUOIA_ASSERT_MSG(camera, "no Camera set");

//  DrawCommandList* drawCommandList = command->getDrawCommandList();
//  SEQUOIA_ASSERT_MSG(drawCommandList, "no DrawCommandList set");

//  // Set the viewport
//  stateCacheManager_->setViewport(viewport);

//  // Compute projection-view matrix
//  math::mat4 matVP = camera->getViewProjectionMatrix();
//  UniformVariable u_matP = camera->getProjectionMatrix();

//  // Clear the screen
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//  // Extract a list of all referenced, valid programs
//  std::unordered_set<Program*> programs;
//  for(DrawCommand* drawCommand = drawCommandList->start(); drawCommand != nullptr;
//      drawCommand = drawCommandList->next()) {
//    programs.insert(drawCommand->getProgram());
//  }

//  // Set the uniform variables which are persistent accross the DrawCommands for each program
//  command->forEachUniformVariable(
//      programs, [this](Program* program, const std::string& name, const UniformVariable& variable) {
//        stateCacheManager_->setUniformVariable(program, name, variable);
//      });

//  for(DrawCommand* drawCommand = drawCommandList->start(); drawCommand != nullptr;
//      drawCommand = drawCommandList->next()) {
//    GLProgram* program = core::dyn_cast<GLProgram>(drawCommand->getProgram());
//    if(!program->isValid())
//      continue;

//    // Compute the transformation matrices (camera to world space)
//    UniformVariable u_matM = drawCommand->getModelMatrix();
//    UniformVariable u_matMVP = matVP * drawCommand->getModelMatrix();
//    program->setUniformVariable("u_matMVP", u_matMVP);
//    program->setUniformVariable("u_matM", u_matM);
//    program->setUniformVariable("u_matP", u_matP);

//    // Update the OpenGL state-machine and draw the command
//    stateCacheManager_->draw(drawCommand);
//  }

//  // Inform everyone that we finished rendering the frame
//  for(FrameListener* listener : getListeners<FrameListener>())
//    listener->frameListenerRenderingEnd(command);

//  // Unbind the FBO
//  if(target->hasFrameBuffer())
//    stateCacheManager_->unbindFrameBufferObject();
//}

} // namespace render

} // namespace sequoia
