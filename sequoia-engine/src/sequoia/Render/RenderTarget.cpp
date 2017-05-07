//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#include "sequoia/Core/Assert.h"
#include "sequoia/Render/RenderTarget.h"
#include "sequoia/Render/Camera.h"

namespace sequoia {

namespace render {

RenderTarget::RenderTarget(RenderTargetKind kind)
    : kind_(kind), active_(true), viewport_(nullptr) {}

void RenderTarget::renderOneFrame() {
  // Compute the model view matrix
  viewport_->getCamera()->updateModelViewMatrix();

  // Render the scene
  update();
}

bool RenderTarget::isActive() const { return active_; }

void RenderTarget::setActive(bool active) { active_ = active; }

Viewport* RenderTarget::getViewport() {
  SEQUOIA_ASSERT(viewport_);
  return viewport_.get();
}

const Viewport* RenderTarget::getViewport() const {
  SEQUOIA_ASSERT(viewport_);
  return viewport_.get();
}

void RenderTarget::setViewport(const std::shared_ptr<Viewport>& viewport) { viewport_ = viewport; }

} // namespace render

} // namespace sequoia
