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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/FrameBufferObject.h"

namespace sequoia {

namespace render {

static const char* modeKindToString(FrameBufferObjectParameter::ModeKind mode) {
  switch(mode) {
  case FrameBufferObjectParameter::MK_RenderToTexture:
    return "RenderToTexture";
  default:
    sequoia_unreachable("invalid mode");
  }
}

std::string FrameBufferObjectParameter::toString() const {
  return core::format("FrameBufferObjectParameter[\n"
                      "  Width = %s,\n"
                      "  Height = %s,\n"
                      "  Mode = %s,\n"
                      "  MSAA = %s\n"
                      "]",
                      Width, Height, modeKindToString(Mode), MSAA);
}

FrameBufferObject::~FrameBufferObject() {}

FrameBufferObject::FrameBufferObject(RenderSystemKind kind) : RenderSystemObject(kind) {}

} // namespace render

} // namespace sequoia