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
                                         const VertexLayout* layout, BufferUsageKind usage,
                                         unsigned int* indicesPtr, std::size_t numIndices) {
  dataPtr_ = dataPtr;
  numVertices_ = numVertices;
  layout_ = layout;
  usage_ = usage;

  indicesPtr_ = indicesPtr;
  numIndices_ = numIndices;

  attachVertexDataDevice();
}

void VertexArrayObject::freeVertexData() {
  freeVertexDataDevice();
  dataPtr_ = nullptr;
  numVertices_ = 0;
  layout_ = nullptr;

  indicesPtr_ = nullptr;
  numIndices_ = 0;
}

std::size_t VertexArrayObject::getNumVertices() const { return numVertices_; }

std::size_t VertexArrayObject::getNumIndices() const { return numIndices_; }

bool VertexArrayObject::hasIndices() const { return indicesPtr_ != nullptr; }

} // namespace render

} // namespace sequoia
