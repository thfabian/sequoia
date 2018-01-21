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

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/D3D12/D3D12Renderer.h"

namespace sequoia {

namespace render {

#define RENDER_STATE(Type, Name, DefaultValue)                                                     \
  bool D3D12Renderer::Name##Changed(Type value) { return true; }
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

bool D3D12Renderer::ProgramChanged(Program* program) { return true; }

bool D3D12Renderer::VertexDataChanged(VertexData* data) { return true; }

bool D3D12Renderer::TextureChanged(int textureUnit, Texture* texture, bool enable) { return true; }

bool D3D12Renderer::UniformVariableChanged(Program* program, const std::string& name,
                                          const UniformVariable& value) {
  return true;
}

bool D3D12Renderer::ViewportChanged(int x, int y, int width, int height) { return true; }

bool D3D12Renderer::clearRenderBuffers(
    const std::set<RenderBuffer::RenderBufferKind>& buffersToClear) {
  return true;
}

bool D3D12Renderer::draw(const DrawCommand& drawCommand) { return true; }

std::pair<std::string, std::string> D3D12Renderer::toStringImpl() const {
  return std::make_pair("D3D12Renderer", core::format("{}", Base::toStringImpl().second));
}

D3D12Renderer::D3D12Renderer() : Renderer(RK_D3D12) {}

} // namespace render

} // namespace sequoia
