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

#ifndef SEQUOIA_ENGINE_RENDER_DRAWCONTEXT_H
#define SEQUOIA_ENGINE_RENDER_DRAWCONTEXT_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/RenderFwd.h"
#include "sequoia-engine/Render/UniformVariable.h"
#include <string>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Information passed to each RenderPass/RenderTechnique during execution of the
/// pass/technique to configure the next sequence of draw calls
///
/// This allows to access and modify the current RenderPipeline.
///
/// @ingroup render
struct SEQUOIA_API DrawContext {
  /// Static scene information
  const DrawScene* Scene;

  /// Current state of the render-pipeline
  ///
  /// Note that the pipeline is reset after each execution of a `RenderTechnique` (but not in
  /// between `RenderPass`es of the same technique!)
  RenderPipeline CurrentPipeline;

  /// Current uniform variables which are bound to the GPU Program of `CurrentPipeline`
  ///
  /// Note that the map is cleared before the execution of each `RenderPass`.
  std::unordered_map<std::string, UniformVariable> CurrentUniforms;

  /// Uniform variables shared by all RenderPass`es of the current `RenderTechnique`
  std::unordered_map<std::string, UniformVariable> SharedUniforms;
};

} // namespace render

} // namespace sequoia

#endif
