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
#include "sequoia/Render/Camera.h"
#include "sequoia/Render/RenderTarget.h"

namespace sequoia {

namespace render {

RenderTarget::RenderTarget(RenderTargetKind kind)
    : kind_(kind), active_(true), viewport_(nullptr) {}

bool RenderTarget::isActive() const { return active_; }

void RenderTarget::setActive(bool active) { active_ = active; }

bool RenderTarget::hasViewport() const { return viewport_ != nullptr; }

Viewport* RenderTarget::getViewport() {
  SEQUOIA_ASSERT(viewport_);
  return viewport_.get();
}

const Viewport* RenderTarget::getViewport() const {
  SEQUOIA_ASSERT(viewport_);
  return viewport_.get();
}

void RenderTarget::setViewport(const std::shared_ptr<Viewport>& viewport) { viewport_ = viewport; }

void RenderTarget::setDrawCommandList(const std::shared_ptr<DrawCommandList>& list) {
  list_ = list;
}

const std::shared_ptr<DrawCommandList>& RenderTarget::getDrawCommandList() { return list_; }

} // namespace render

} // namespace sequoia
