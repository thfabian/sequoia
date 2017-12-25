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

#ifndef SEQUOIA_ENGINE_RENDER_RTDEFAULT_H
#define SEQUOIA_ENGINE_RENDER_RTDEFAULT_H

#include "sequoia-engine/Render/RenderPass.h"
#include "sequoia-engine/Render/RenderTechnique.h"
#include <memory>

namespace sequoia {

namespace render {

/// @brief Default RenderTechnique
/// @ingroup render
class RTDefault : public RenderTechnique {
  std::unique_ptr<RenderPass> defaultPass_;

public:
  RTDefault();

  /// @brief Simple render pass
  virtual void render(std::function<void(RenderPass*)> renderer) override;

  /// @brief Name of the technique
  virtual const char* getName() const noexcept override;
};

} // namespace render

} // namespace sequoia

#endif
