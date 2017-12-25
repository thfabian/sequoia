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
#include "sequoia-engine/Render/GL/GLPixelFormat.h"

namespace sequoia {

namespace render {

int GLPixelFormat::get(GLenum param) const noexcept {
  auto it = format_.find(param);
  SEQUOIA_ASSERT(format_.end() != it);
  return it->second;
}

void GLPixelFormat::set(GLenum param, int value) noexcept { format_[param] = value; }

std::string GLPixelFormat::toString() const {
  return core::format("GLPixelFormat[\n"
                      "  format = {}\n"
                      "]",
                      core::indent(core::toStringRange(format_, [](auto pair) {
                        return core::format("{} = {}", pair.first, pair.second);
                      })));
}

} // namespace render

} // namespace sequoia
