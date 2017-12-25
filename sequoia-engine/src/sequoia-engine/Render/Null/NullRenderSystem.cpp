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

#include "sequoia-engine/Render/Null/NullInputSystem.h"
#include "sequoia-engine/Render/Null/NullProgram.h"
#include "sequoia-engine/Render/Null/NullRenderSystem.h"
#include "sequoia-engine/Render/Null/NullRenderWindow.h"
#include "sequoia-engine/Render/Null/NullRenderer.h"
#include "sequoia-engine/Render/Null/NullShader.h"
#include "sequoia-engine/Render/Null/NullTexture.h"
#include "sequoia-engine/Render/Null/NullVertexData.h"

namespace sequoia {

namespace render {

NullRenderSystem::NullRenderSystem(const std::shared_ptr<Options>& options)
    : RenderSystem(RK_Null, options, ShaderSourceManager::SL_GLSL), mainWindow_(nullptr) {
  inputSystem_ = std::make_unique<NullInputSystem>();
}

NullRenderSystem::~NullRenderSystem() {}

RenderWindow* NullRenderSystem::createMainWindow(const RenderWindow::WindowHint& hints) {
  destroyMainWindow();
  mainWindow_ = std::make_unique<NullRenderWindow>(hints);
  renderer_ = std::make_unique<NullRenderer>();
  return mainWindow_.get();
}

void NullRenderSystem::destroyMainWindow() noexcept {
  renderer_.reset();
  renderer_ = nullptr;

  mainWindow_.reset();
  mainWindow_ = nullptr;
}

RenderWindow* NullRenderSystem::getMainWindow() const {
  SEQUOIA_ASSERT_MSG(mainWindow_, "main window not initialized");
  return mainWindow_.get();
}

void NullRenderSystem::pollEvents() {}

std::shared_ptr<Shader> NullRenderSystem::createShader(Shader::ShaderType type,
                                                       const std::string& filename,
                                                       const std::string& source) {
  return std::make_shared<NullShader>(type, filename, source);
}

std::shared_ptr<Program>
NullRenderSystem::createProgram(const std::set<std::shared_ptr<Shader>>& shaders) {
  return std::make_shared<NullProgram>(shaders);
}

std::shared_ptr<Texture> NullRenderSystem::createTexture(const std::shared_ptr<Image>& image,
                                                         const TextureParameter& param) {
  return std::make_shared<NullTexture>(image, std::make_shared<TextureParameter>(param));
}

std::shared_ptr<VertexData> NullRenderSystem::createVertexData(const VertexDataParameter& param) {
  return std::make_shared<NullVertexData>(param);
}

void NullRenderSystem::addKeyboardListener(KeyboardListener* listener) {
  inputSystem_->addListener<KeyboardListener>(listener);
}

void NullRenderSystem::removeKeyboardListener(KeyboardListener* listener) {
  inputSystem_->removeListener<KeyboardListener>(listener);
}

void NullRenderSystem::addMouseListener(MouseListener* listener) {
  inputSystem_->addListener<MouseListener>(listener);
}

void NullRenderSystem::removeMouseListener(MouseListener* listener) {
  inputSystem_->removeListener<MouseListener>(listener);
}

Renderer* NullRenderSystem::getRenderer() const { return renderer_.get(); }

} // namespace render

} // namespace sequoia
