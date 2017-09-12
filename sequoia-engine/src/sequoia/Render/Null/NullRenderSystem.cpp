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

#include "sequoia/Render/Null/NullRenderSystem.h"
#include "sequoia/Render/Null/NullInputSystem.h"
#include "sequoia/Render/Null/NullProgram.h"
#include "sequoia/Render/Null/NullRenderWindow.h"
#include "sequoia/Render/Null/NullShader.h"
#include "sequoia/Render/Null/NullTexture.h"
#include "sequoia/Render/Null/NullVertexData.h"

namespace sequoia {

namespace render {

NullRenderSystem::NullRenderSystem(Options* options)
    : RenderSystem(RK_Null, options), mainWindow_(nullptr) {
  inputSystem_ = std::make_unique<NullInputSystem>();
}

NullRenderSystem::~NullRenderSystem() {}

RenderWindow* NullRenderSystem::createMainWindow(const RenderWindow::WindowHint& hints) {
  SEQUOIA_ASSERT_MSG(!mainWindow_, "main window already initialized");
  mainWindow_ = std::make_unique<NullRenderWindow>(hints);
  return mainWindow_.get();
}

void NullRenderSystem::destroyMainWindow() noexcept {
  mainWindow_.reset();
  mainWindow_ = nullptr;
}

RenderWindow* NullRenderSystem::getMainWindow() const {
  SEQUOIA_ASSERT_MSG(!mainWindow_, "main window not initialized");
  return mainWindow_.get();
}

void NullRenderSystem::pollEvents() {}

void NullRenderSystem::renderOneFrame(RenderCommand* command) {}

std::shared_ptr<Shader> NullRenderSystem::createShader(Shader::ShaderType type,
                                                       const std::shared_ptr<File>& path) {
  return std::make_shared<NullShader>(type, path);
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

void NullRenderSystem::loadDefaultShaders(const std::shared_ptr<File>& defaultVertexShaderFile,
                                          const std::shared_ptr<File>& defaultFragmentShaderFile) {
  defaultVertexShader_ = std::make_shared<NullShader>(Shader::ST_Vertex, defaultVertexShaderFile);
  defaultFragmentShader_ =
      std::make_shared<NullShader>(Shader::ST_Fragment, defaultFragmentShaderFile);
  defaultProgram_ = std::make_shared<NullProgram>(
      std::set<std::shared_ptr<Shader>>{defaultVertexShader_, defaultFragmentShader_});
}

const std::shared_ptr<Shader>& NullRenderSystem::getDefaultVertexShader() const {
  return defaultVertexShader_;
}

const std::shared_ptr<Shader>& NullRenderSystem::getDefaultFragmentShader() const {
  return defaultFragmentShader_;
}

const std::shared_ptr<Program>& NullRenderSystem::getDefaultProgram() const {
  return defaultProgram_;
}

} // namespace render

} // namespace sequoia
