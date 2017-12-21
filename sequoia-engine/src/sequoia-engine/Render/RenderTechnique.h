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
#include "sequoia-engine/Render/RenderPass.h"
#include <vector>

namespace sequoia {

namespace render {

/// @brief Implementation of a specific render technique to enable various visual effects
///
/// A `RenderTechnique` encompasses multiple `RenderPass`es which which are executed by the
/// renderer. A RenderTechnique should be render backend agnostic.
///
/// @ingroup render
class SEQUOIA_API RenderTechnique {
public:
  virtual ~RenderTechnique() {}

  /// @brief Called right before the technqiue is being executed
  virtual void setUp() {}

  /// @brief Called right after the technique has finished executing
  virtual void tearDown() {}

  /// @brief Get the name of the technique
  virtual const char* getName() const noexcept = 0;

  /// @brief Get the render passes
  std::vector<RenderPass*>& getPasses() noexcept { return passes_; }

protected:
  std::vector<RenderPass*> passes_;
};

} // namespace render

} // namespace sequoia

#endif
