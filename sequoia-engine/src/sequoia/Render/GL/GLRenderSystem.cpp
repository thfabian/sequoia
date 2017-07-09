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
#include "sequoia/Core/ErrorHandler.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLInputSystem.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Render/GL/GLTextureManager.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"

namespace sequoia {

namespace render {

static void GLFWErrorCallbackSoft(int error, const char* description) {
  LOG(ERROR) << "GLFW error: " << description;
}

static void GLFWErrorCallbackHard(int error, const char* description) {
  GLFWErrorCallbackSoft(error, description);
  SEQUOIA_THROW(RenderSystemException, description);
}

GLRenderSystem::GLRenderSystem()
    : RenderSystem(RK_OpenGL), mainWindow_(nullptr), renderer_(nullptr) {
  LOG(INFO) << "Initializing OpenGL RenderSystem ...";
  glfwSetErrorCallback(GLFWErrorCallbackHard);

  glfwInit();
  LOG(INFO) << "GLFW: " << glfwGetVersionString();

  glfwSetErrorCallback(GLFWErrorCallbackSoft);
  LOG(INFO) << "Done initializing OpenGL RenderSystem";
}

GLRenderSystem::~GLRenderSystem() {
  LOG(INFO) << "Terminating OpenGL RenderSystem ...";
  destroyMainWindow();
  glfwTerminate();
  LOG(INFO) << "Done terminating OpenGL RenderSystem";
}

RenderWindow* GLRenderSystem::createMainWindow(const RenderWindow::WindowHint& hints) {
  // Reset everything
  destroyMainWindow();

  // Create main-window and OpenGL context
  mainWindow_ = std::make_unique<GLRenderWindow>(hints);

  // Initialize OpenGL renderer
  renderer_ = std::make_unique<GLRenderer>(mainWindow_.get());
  renderer_->addListener<FrameListener>(this);

  return mainWindow_.get();
}

RenderWindow* GLRenderSystem::getMainWindow() const { return getMainGLWindow(); }

GLRenderWindow* GLRenderSystem::getMainGLWindow() const { return mainWindow_.get(); }

void GLRenderSystem::destroyMainWindow() noexcept {
  // Order matters here!
  if(renderer_) {
    renderer_.reset();
    renderer_ = nullptr;
  }

  if(mainWindow_) {
    mainWindow_.reset();
    mainWindow_ = nullptr;
  }
}

void GLRenderSystem::pollEvents() {
  for(auto* listener : getListeners<InputEventListener>())
    listener->inputEventStart();

  glfwPollEvents();

  for(auto* listener : getListeners<InputEventListener>())
    listener->inputEventStop();
}

void GLRenderSystem::renderOneFrame(RenderTarget* target) { getRenderer()->render(target); }

std::unique_ptr<VertexArrayObject> GLRenderSystem::createVertexArrayObject() {
  return std::make_unique<GLVertexArrayObject>();
}

std::shared_ptr<Shader> GLRenderSystem::createShader(Shader::ShaderType type,
                                                     const std::shared_ptr<File>& file) {
  std::shared_ptr<GLShader> shader = getRenderer()->getShaderManager()->create(type, file);
  shader->makeValid();
  return shader;
}

std::shared_ptr<Program>
GLRenderSystem::createProgram(const std::set<std::shared_ptr<Shader>>& shaders) {
  std::shared_ptr<GLProgram> program = getRenderer()->getProgramManager()->create(shaders);
  program->makeValid();
  return program;
}

std::shared_ptr<Texture> GLRenderSystem::createTexture(const std::shared_ptr<Image>& image,
                                                       const TextureParameter& param) {
  std::shared_ptr<GLTexture> texture =
      getRenderer()->getTextureManager()->create(image, std::make_shared<TextureParameter>(param));
  texture->makeValid();
  return texture;
}

void GLRenderSystem::addKeyboardListener(KeyboardListener* listener) {
  mainWindow_->getInputSystem()->addListener(listener);
}

void GLRenderSystem::removeKeyboardListener(KeyboardListener* listener) {
  mainWindow_->getInputSystem()->removeListener(listener);
}

void GLRenderSystem::addMouseListener(MouseListener* listener) {
  mainWindow_->getInputSystem()->addListener(listener);
}

void GLRenderSystem::removeMouseListener(MouseListener* listener) {
  mainWindow_->getInputSystem()->removeListener(listener);
}

void GLRenderSystem::loadDefaultShaders(const std::shared_ptr<File>& defaultVertexShaderFile,
                                        const std::shared_ptr<File>& defaultFragmentShaderFile) {
  getRenderer()->loadDefaultShaders(defaultVertexShaderFile, defaultFragmentShaderFile);
}

const std::shared_ptr<Shader>& GLRenderSystem::getDefaultVertexShader() const {
  return getRenderer()->getDefaultVertexShader();
}

const std::shared_ptr<Shader>& GLRenderSystem::getDefaultFragmentShader() const {
  return getRenderer()->getDefaultFragmentShader();
}

const std::shared_ptr<Program>& GLRenderSystem::getDefaultProgram() const {
  return getRenderer()->getDefaultProgram();
}

GLRenderer* GLRenderSystem::getRenderer() { return renderer_.get(); }
GLRenderer* GLRenderSystem::getRenderer() const { return renderer_.get(); }

GLStateCacheManager* GLRenderSystem::getStateCacheManager() {
  return getRenderer()->getStateCacheManager();
}

GLRenderSystem& getGLRenderSystem() noexcept { return *getGLRenderSystemPtr(); }

GLRenderSystem* getGLRenderSystemPtr() noexcept {
  return dyn_cast<GLRenderSystem>(RenderSystem::getSingletonPtr());
}

} // namespace render

} // namespace sequoia
