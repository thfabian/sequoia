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

#include "sequoia/Render/VertexArrayObject.h"

namespace sequoia {

namespace render {

VertexArrayObject::~VertexArrayObject() {}

VertexArrayObject::VertexArrayObject(RenderSystemKind kind)
    : RenderSystemObject(kind), dataPtr_(nullptr), numVertices_(0) {}

} // namespace render

} // namespace sequoia
