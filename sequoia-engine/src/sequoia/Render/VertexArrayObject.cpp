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
    : RenderSystemObject(kind), dataPtr_(nullptr), numVertices_(0), layout_(nullptr),
      usage_(BK_Invalid) {}

void VertexArrayObject::attachVertexData(void* dataPtr, std::size_t numVertices,
                                         const VertexLayout* layout, BufferUsageKind usage) {
  dataPtr_ = dataPtr;
  numVertices_ = numVertices;
  layout_ = layout;
  usage_ = usage;
  attachVertexDataDevice();
}

void VertexArrayObject::freeVertexData() {
  freeVertexDataDevice();
  dataPtr_ = nullptr;
  numVertices_ = 0;
  layout_ = nullptr;
}

std::size_t VertexArrayObject::getNumVertices() const { return numVertices_; }

} // namespace render

} // namespace sequoia
