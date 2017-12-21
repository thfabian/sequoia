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

#ifndef SEQUOIA_ENGINE_RENDER_DRAWCALLCONTEXT_H
#define SEQUOIA_ENGINE_RENDER_DRAWCALLCONTEXT_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include "sequoia-engine/Render/RenderPipeline.h"
#include <string>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Information on how to render a sequence of `DrawCommand`s in a `RenderPass`
///
/// Note that the DrawCallContext is newly constructed before each `RenderPass::setUp` call.
/// Hence, no information can be shared in between RenderPasses via DrawCallContext.
///
/// @ingroup render
struct SEQUOIA_API DrawCallContext : public NonCopyable {
  DrawCallContext(render::Viewport* viewport, const RenderTarget* target, const DrawScene* scene,
                  const std::vector<DrawCommand*>* drawCommands)
      : Viewport(viewport), Target(target), Scene(scene), DrawCommands(drawCommands) {}

  /// Viewport (including the Camera)
  ///
  /// By default, this is set to the Viewport of the `Target`
  render::Viewport* Viewport;

  /// State of the render-pipeline (including the GPU program)
  render::RenderPipeline Pipeline;

  /// Uniform variables which are going to be bound to the GPU program of `Pipeline`
  ///
  /// Note that the in addition to these uniform variables, the per DrawCommand uniforms, i.e the
  /// the model-view-projection matrix as well as user defined variables, are sent automatically to
  /// the program.
  std::unordered_map<std::string, UniformVariable> Uniforms;

  /// Clear the color buffer?
  bool ClearColorBuffer = true;

  /// Clear the depth buffer?
  bool ClearDepthBuffer = true;

  /// Clear the stencil buffer?
  bool ClearStencilBuffer = true;

  /// Global render target
  const render::RenderTarget* Target;

  /// Static scene information
  const render::DrawScene* Scene;

  /// Sequence of `DrawCommand`s which are going to be invoked
  const std::vector<DrawCommand*>* DrawCommands;
};

} // namespace render

} // namespace sequoia

#endif
