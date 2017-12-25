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
#include "sequoia-engine/Render/Null/NullRenderer.h"

namespace sequoia {

namespace render {

#define RENDER_STATE(Type, Name, DefaultValue)                                                     \
  bool NullRenderer::Name##Changed(Type value) { return true; }
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

bool NullRenderer::ProgramChanged(Program* program) { return true; }

bool NullRenderer::VertexDataChanged(VertexData* data) { return true; }

bool NullRenderer::TextureChanged(int textureUnit, Texture* texture, bool enable) { return true; }

bool NullRenderer::UniformVariableChanged(Program* program, const std::string& name,
                                          const UniformVariable& value) {
  return true;
}

bool NullRenderer::ViewportChanged(int x, int y, int width, int height) { return true; }

bool NullRenderer::clearRenderBuffers(
    const std::set<RenderBuffer::RenderBufferKind>& buffersToClear) {
  return true;
}

bool NullRenderer::draw(const DrawCommand& drawCommand) { return true; }

std::pair<std::string, std::string> NullRenderer::toStringImpl() const {
  return std::make_pair("NullRenderer", core::format("{}", Base::toStringImpl().second));
}

NullRenderer::NullRenderer() : Renderer(RK_Null) {}

} // namespace render

} // namespace sequoia
