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
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderWindow.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Render/GL/GLTextureManager.h"
#include "sequoia/Render/GL/GLVertexArrayObject.h"
#include "sequoia/Render/GL/Native.h"

namespace sequoia {

namespace render {

namespace {

/// @brief Create the ressource of type `T` (where `T` is one of (GLShader, GLTexture or GLProgram)
/// using `manager`
///
/// @param manager  Manager used to create the ressource
/// @param args     Arguments passed to `manger->create(...)`
/// @returns ressource whith `ressource->isValid() == true` or the exception caught during
///          construction of the ressource will be rethrown
template <class T, class ManagerType, class... Args>
std::shared_ptr<T> createRessource(ManagerType* manager, Args&&... args) {
  std::shared_ptr<T> ressource = manager->create(std::forward<Args>(args)...);

  if(ressource->isValid())
    return ressource;

  // Convert the ressource into a valid state
  ressource->makeValid();

  if(ressource->hasException()) {
    // Remove the ressource from the manager and rethrow the exception
    manager->remove(ressource);
    ressource->rethrowException();
    sequoia_unreachable("captured exception not rethrown");
  }

  return ressource;
}

} // anonymous namespace

GLRenderSystem::GLRenderSystem()
    : RenderSystem(RK_OpenGL), mainContext_(nullptr), mainWindow_(nullptr), renderer_(nullptr) {
  LOG(INFO) << "Initializing OpenGL RenderSystem ...";
}

GLRenderSystem::~GLRenderSystem() {
  LOG(INFO) << "Terminating OpenGL RenderSystem ...";
  destroyMainWindow();
  LOG(INFO) << "Done terminating OpenGL RenderSystem";
}

RenderWindow* GLRenderSystem::createMainWindow(const RenderWindow::WindowHint& hints) {
  // Reset everything
  destroyMainWindow();

  // Initialize the main OpenGL context
  mainContext_ = NativeGLContext::create(NativeWindowSystemKind::NK_GLFW3);
  mainContext_->init(hints);

  // Create main-window and OpenGL context
  mainWindow_ = std::make_unique<GLRenderWindow>(mainContext_);

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

  if(mainContext_) {
    mainContext_.reset();
    mainContext_ = nullptr;
  }
}

void GLRenderSystem::pollEvents() {
  for(auto* listener : getListeners<InputEventListener>())
    listener->inputEventStart();

  mainWindow_->getInputSystem()->pollEvents();

  for(auto* listener : getListeners<InputEventListener>())
    listener->inputEventStop();
}

void GLRenderSystem::renderOneFrame(RenderTarget* target) { getRenderer()->render(target); }

std::unique_ptr<VertexArrayObject> GLRenderSystem::createVertexArrayObject() {
  return std::make_unique<GLVertexArrayObject>();
}

std::shared_ptr<Shader> GLRenderSystem::createShader(Shader::ShaderType type,
                                                     const std::shared_ptr<File>& file) {
  return createRessource<GLShader>(getRenderer()->getShaderManager(), type, file);
}

std::shared_ptr<Program>
GLRenderSystem::createProgram(const std::set<std::shared_ptr<Shader>>& shaders) {
  return createRessource<GLProgram>(getRenderer()->getProgramManager(), shaders);
}

std::shared_ptr<Texture> GLRenderSystem::createTexture(const std::shared_ptr<Image>& image,
                                                       const TextureParameter& param) {
  return createRessource<GLTexture>(getRenderer()->getTextureManager(), image,
                                    std::make_shared<TextureParameter>(param));
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
