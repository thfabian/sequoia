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

#include "sequoia/Render/VertexArrayObject.h"
#include "sequoia/Render/VertexData.h"

namespace sequoia {

namespace render {

VertexArrayObject::~VertexArrayObject() {}

VertexArrayObject::VertexArrayObject(RenderSystemKind kind)
    : RenderSystemObject(kind), data_(nullptr), usage_(BK_Invalid) {}

void VertexArrayObject::attachVertexData(VertexData* data, BufferUsageKind usage) {
  data_ = data;
  usage_ = usage;
  attachVertexDataDevice();
}

void VertexArrayObject::freeVertexData() { freeVertexDataDevice(); }

std::size_t VertexArrayObject::getNumVertices() const { return data_->getNumVertices(); }

std::size_t VertexArrayObject::getNumIndices() const { return data_->getNumIndices(); }

bool VertexArrayObject::hasIndices() const { return data_->hasIndices(); }

VertexData* VertexArrayObject::getVertexData() { return data_; }

} // namespace render

} // namespace sequoia
