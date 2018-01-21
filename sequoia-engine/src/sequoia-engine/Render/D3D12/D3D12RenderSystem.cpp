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

#include "sequoia-engine/Render/D3D12/D3D12Program.h"
#include "sequoia-engine/Render/D3D12/D3D12RenderSystem.h"
#include "sequoia-engine/Render/D3D12/D3D12RenderWindow.h"
#include "sequoia-engine/Render/D3D12/D3D12Renderer.h"
#include "sequoia-engine/Render/D3D12/D3D12Shader.h"
#include "sequoia-engine/Render/D3D12/D3D12Texture.h"
#include "sequoia-engine/Render/D3D12/D3D12VertexData.h"

namespace sequoia {

namespace render {

D3D12RenderSystem::D3D12RenderSystem(const std::shared_ptr<Options>& options)
    : RenderSystem(RK_D3D12, options, ShaderSourceManager::SL_HLSL), mainWindow_(nullptr) {}

D3D12RenderSystem::~D3D12RenderSystem() {}

RenderWindow* D3D12RenderSystem::createMainWindow(const RenderWindow::WindowHint& hints) {
  destroyMainWindow();
  mainWindow_ = std::make_unique<D3D12RenderWindow>(hints);
  renderer_ = std::make_unique<D3D12Renderer>();
  return mainWindow_.get();
}

void D3D12RenderSystem::destroyMainWindow() noexcept {
  renderer_.reset();
  renderer_ = nullptr;

  mainWindow_.reset();
  mainWindow_ = nullptr;
}

RenderWindow* D3D12RenderSystem::getMainWindow() const {
  SEQUOIA_ASSERT_MSG(mainWindow_, "main window not initialized");
  return mainWindow_.get();
}

void D3D12RenderSystem::pollEvents() { mainWindow_->pollEvents(); }

std::shared_ptr<Shader> D3D12RenderSystem::createShader(Shader::ShaderType type,
                                                        const std::string& filename,
                                                        const std::string& source) {
  return std::make_shared<D3D12Shader>(type, filename, source);
}

std::shared_ptr<Program>
D3D12RenderSystem::createProgram(const std::set<std::shared_ptr<Shader>>& shaders) {
  return std::make_shared<D3D12Program>(shaders);
}

std::shared_ptr<Texture> D3D12RenderSystem::createTexture(const std::shared_ptr<Image>& image,
                                                         const TextureParameter& param) {
  return std::make_shared<D3D12Texture>(image, std::make_shared<TextureParameter>(param));
}

std::shared_ptr<VertexData> D3D12RenderSystem::createVertexData(const VertexDataParameter& param) {
  return std::make_shared<D3D12VertexData>(param);
}

void D3D12RenderSystem::addKeyboardListener(KeyboardListener* listener) {
  mainWindow_->addListener<KeyboardListener>(listener);
}

void D3D12RenderSystem::removeKeyboardListener(KeyboardListener* listener) {
  mainWindow_->removeListener<KeyboardListener>(listener);
}

void D3D12RenderSystem::addMouseListener(MouseListener* listener) {
  mainWindow_->addListener<MouseListener>(listener);
}

void D3D12RenderSystem::removeMouseListener(MouseListener* listener) {
  mainWindow_->removeListener<MouseListener>(listener);
}

Renderer* D3D12RenderSystem::getRenderer() const { return renderer_.get(); }

} // namespace render

} // namespace sequoia
