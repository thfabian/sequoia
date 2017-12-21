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

#ifndef SEQUOIA_ENGINE_RENDER_NULL_NULLRENDERER_H
#define SEQUOIA_ENGINE_RENDER_NULL_NULLRENDERER_H

#include "sequoia-engine/Render/Renderer.h"

namespace sequoia {

namespace render {

/// @brief Null Renderer implementation
/// @ingroup null
class SEQUOIA_API NullRenderer final : public Renderer {
protected:
  using Base = Renderer;

#define RENDER_STATE(Type, Name, DefaultValue) virtual bool Name##Changed(Type value) override;
#include "sequoia-engine/Render/RenderState.inc"
#undef RENDER_STATE

  /// @copydoc Renderer::ProgramChanged
  virtual bool ProgramChanged(Program* program) override;

  /// @copydoc Renderer::VertexDataChanged
  virtual bool VertexDataChanged(VertexData* data, bool bindForDrawing) override;

  /// @copydoc Renderer::TextureChanged
  virtual bool TextureChanged(int textureUnit, Texture* texture, bool enable) override;

  /// @copydoc Renderer::UniformVariableChanged
  virtual bool UniformVariableChanged(Program* program, const std::string& name,
                                      const UniformVariable& value) override;

  /// @copydoc Renderer::ViewportChanged
  virtual bool ViewportChanged(int x, int y, int width, int height) override;

  /// @copydoc Renderer::clearColorBuffer
  virtual bool clearColorBuffer() override;

  /// @copydoc Renderer::clearDepthBuffer
  virtual bool clearDepthBuffer() override;

  /// @copydoc Renderer::clearStencilBuffer
  virtual bool clearStencilBuffer() override;

  /// @copydoc Renderer::draw
  virtual bool draw(const DrawCommand* drawCommand) override;

  /// @copydoc Renderer::toStringImpl
  std::pair<std::string, std::string> toStringImpl() const override;

public:
  NullRenderer();
};

} // namespace render

} // namespace sequoia

#endif
