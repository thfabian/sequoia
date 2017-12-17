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
#include "sequoia-engine/Render/RenderPassContext.h"

namespace sequoia {

namespace render {

/// @brief
/// @ingroup render
class SEQUOIA_API RenderPass {
public:
  virtual ~RenderPass();
  
  /// @brief Executed right before rendering of this pass starts
  virtual void renderingBegin(const RenderPassContext& ctx) = 0;

  /// @brief Executed right after rendering of this pass has finished
  virtual void renderingEnd(const RenderPassContext& ctx) = 0;

protected:
  RenderState state_;
};

} // namespace render

} // namespace sequoia

#endif
