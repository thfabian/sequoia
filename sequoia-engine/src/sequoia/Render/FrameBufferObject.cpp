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
#include "sequoia/Render/FrameBufferObject.h"

namespace sequoia {

namespace render {

std::string FrameBufferObjectParameter::toString() const {
  return core::format("FrameBufferObjectParameter[\n"
                      "  Width = %s,\n"
                      "  Height = %s,\n"
                      "  MSAA = %s\n"
                      "]",
                      Width, Height, MSAA);
}

FrameBufferObject::~FrameBufferObject() {}

FrameBufferObject::FrameBufferObject(RenderSystemKind kind) : RenderSystemObject(kind) {}

} // namespace render

} // namespace sequoia
