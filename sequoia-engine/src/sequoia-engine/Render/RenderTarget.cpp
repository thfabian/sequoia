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

#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/RenderTarget.h"
#include "sequoia-engine/Render/Viewport.h"

namespace sequoia {

namespace render {

RenderTarget::RenderTarget(RenderTargetKind kind) : kind_(kind), viewport_(nullptr) {}

Viewport* RenderTarget::getViewport() {
  SEQUOIA_ASSERT(viewport_);
  return viewport_.get();
}

const Viewport* RenderTarget::getViewport() const {
  SEQUOIA_ASSERT(viewport_);
  return viewport_.get();
}

std::string RenderTarget::toString() const {
  return core::format("RenderTarget[\n"
                      "  viewport = {},\n"
                      "]",
                      core::indent(viewport_->toString()));
}

} // namespace render

} // namespace sequoia
