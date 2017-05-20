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

#include "sequoia/Render/VertexArray.h"

namespace sequoia {

namespace render {

VertexArray::~VertexArray() {}

VertexArray::VertexArray(RenderSystemKind kind)
    : RenderSystemObject(kind), vertices_(nullptr), numVertices_(0) {}

void VertexArray::allocate(std::size_t numVertices) { 
  
  allocateGPU(numVertices); 
}

} // namespace render

} // namespace sequoia
