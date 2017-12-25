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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERTECHNIQUE_H
#define SEQUOIA_ENGINE_RENDER_RENDERTECHNIQUE_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/RenderPass.h"
#include <functional>
#include <vector>

namespace sequoia {

namespace render {

/// @brief Implementation of a specific render technique to enable various visual effects
///
/// A `RenderTechnique` usually encompasses multiple `RenderPass`es which can be **rendered** by
/// using a render-callback function provided by the Renderer.
///
/// A RenderTechnique should be render backend agnostic.
///
/// @ingroup render
class SEQUOIA_API RenderTechnique : public NonCopyable {
public:
  virtual ~RenderTechnique() {}

  /// @brief Called with a `renderer` which can be used to **render** all RenderPasses
  virtual void render(std::function<void(RenderPass*)> renderer) = 0;

  /// @brief Get the name of the technique
  virtual const char* getName() const noexcept = 0;
};

} // namespace render

} // namespace sequoia

#endif
