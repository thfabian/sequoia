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
#include "sequoia-engine/Core/ErrorHandler.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Render/GL/GLProgramManager.h"
#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/GL/GLRenderWindow.h"
#include "sequoia-engine/Render/GL/GLRenderer.h"
#include "sequoia-engine/Render/GL/GLShaderManager.h"
#include "sequoia-engine/Render/GL/GLTextureManager.h"
#include "sequoia-engine/Render/GL/GLVertexData.h"
#include "sequoia-engine/Render/GL/Native.h"

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

GLRenderSystem::GLRenderSystem(const std::shared_ptr<Options>& options)
    : RenderSystem(RK_OpenGL, options, ShaderSourceManager::SL_GLSL), mainContext_(nullptr),
      mainWindow_(nullptr), renderer_(nullptr) {
  Log::info("Initializing OpenGL RenderSystem ...");
}

GLRenderSystem::~GLRenderSystem() {
  Log::info("Terminating OpenGL RenderSystem ...");
  destroyMainWindow();
  Log::info("Done terminating OpenGL RenderSystem");
}

RenderWindow* GLRenderSystem::createMainWindow(const RenderWindow::WindowHint& hints) {
  // Reset everything
  destroyMainWindow();

  // Initialize the main OpenGL context
  mainContext_ = NativeGLContext::create(NativeWindowSystemKind::NK_GLFW3);
  mainContext_->init(hints, getOptionsPtr());

  // Create main-window and OpenGL context
  mainWindow_ = std::make_unique<GLRenderWindow>(mainContext_);

  // Initialize OpenGL renderer
  renderer_ = std::make_unique<GLRenderer>(mainWindow_.get(), getOptions());

  return mainWindow_.get();
}

RenderWindow* GLRenderSystem::getMainWindow() const { return getMainGLWindow(); }

GLRenderWindow* GLRenderSystem::getMainGLWindow() const {
  SEQUOIA_ASSERT_MSG(mainWindow_, "main window not initialized");
  return mainWindow_.get();
}

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

std::shared_ptr<Shader> GLRenderSystem::createShader(Shader::ShaderType type,
                                                     const std::string& filename,
                                                     const std::string& source) {
  return createRessource<GLShader>(getGLRenderer()->getShaderManager(), type, filename, source);
}

std::shared_ptr<Program>
GLRenderSystem::createProgram(const std::set<std::shared_ptr<Shader>>& shaders) {
  return createRessource<GLProgram>(getGLRenderer()->getProgramManager(), shaders);
}

std::shared_ptr<Texture> GLRenderSystem::createTexture(const std::shared_ptr<Image>& image,
                                                       const TextureParameter& param) {
  return createRessource<GLTexture>(getGLRenderer()->getTextureManager(), image,
                                    std::make_shared<TextureParameter>(param));
}

std::shared_ptr<VertexData> GLRenderSystem::createVertexData(const VertexDataParameter& param) {
  return std::make_shared<GLVertexData>(param);
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

Renderer* GLRenderSystem::getRenderer() const { return renderer_.get(); }
GLRenderer* GLRenderSystem::getGLRenderer() const { return renderer_.get(); }

GLRenderSystem& getGLRenderSystem() noexcept { return *getGLRenderSystemPtr(); }

GLRenderSystem* getGLRenderSystemPtr() noexcept {
  return core::dyn_cast<GLRenderSystem>(RenderSystem::getSingletonPtr());
}

} // namespace render

} // namespace sequoia
