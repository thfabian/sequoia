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

#include "sequoia-engine/Render/FrameBuffer.h"
#include "sequoia-engine/Core/Format.h"

namespace sequoia {

namespace render {

std::string FrameBufferParameter::toString() const {
  return core::format("FrameBufferObjectParameter[\n"
                      "  Width = {},\n"
                      "  Height = {},\n"
                      "  MSAA = {}\n"
                      "]",
                      Width, Height, MSAA);
}

FrameBuffer::~FrameBuffer() {}

FrameBuffer::FrameBuffer(RenderSystemKind kind) : RenderSystemObject(kind) {}

} // namespace render

} // namespace sequoia
