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

#include "sequoia/Core/Assert.h"
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/DrawCommandList.h"
#include "sequoia/Render/RenderTarget.h"

namespace sequoia {

namespace render {

RenderTarget::RenderTarget(RenderTargetKind kind) : kind_(kind), viewport_(nullptr) {
  list_ = std::make_shared<render::DrawCommandListDefault>();
}

Viewport* RenderTarget::getViewport() {
  SEQUOIA_ASSERT(viewport_);
  return viewport_.get();
}

const Viewport* RenderTarget::getViewport() const {
  SEQUOIA_ASSERT(viewport_);
  return viewport_.get();
}

} // namespace render

} // namespace sequoia
