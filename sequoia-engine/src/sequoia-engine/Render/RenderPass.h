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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERPASS_H
#define SEQUOIA_ENGINE_RENDER_RENDERPASS_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/DrawContext.h"
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Description of a render pass
/// @ingroup render
class SEQUOIA_API RenderPass {
public:
  virtual ~RenderPass() {}

  /// @brief Called right before the pass is being executed
  ///
  /// @param ctx    Context used in rendering `DrawCommand`s of this pass. The context is initially
  ///               passed to the encompassing `RenderTechnique` and subsequently modified by the
  ///               passes of the technique.
  virtual void setUp(DrawContext& ctx) = 0;

  /// @brief Called right after the pass has finished executing
  virtual void tearDown(DrawContext& ctx) {}
};

} // namespace render

} // namespace sequoia

#endif
