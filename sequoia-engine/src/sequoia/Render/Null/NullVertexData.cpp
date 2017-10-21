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

#include "sequoia/Render/Null/NullVertexData.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/HostBuffer.h"

namespace sequoia {

namespace render {

NullVertexData::NullVertexData(const VertexDataParameter& param)
    : VertexData(RK_Null, param.DrawMode) {

  vertexBuffer_ = std::make_unique<NullVertexBuffer>(param.Layout, param.NumVertexBuffers);
  if(param.NumIndices > 0)
    indexBuffer_ = std::make_unique<NullIndexBuffer>(param.IndexType);

  // Allocate buffers
  vertexBuffer_->allocateVertices(param.NumVertices, param.VertexBufferUsageHint);
  if(indexBuffer_)
    indexBuffer_->allocateIndices(param.NumIndices);

  // Allocate shadow buffers
  if(param.UseVertexShadowBuffer)
    vertexBuffer_->setShadowBuffer(HostBuffer::create(vertexBuffer_->getNumBytes()));

  if(indexBuffer_ && param.UseIndexShadowBuffer)
    indexBuffer_->setShadowBuffer(HostBuffer::create(indexBuffer_->getNumBytes()));
}

NullVertexData::~NullVertexData() {}

VertexBuffer* NullVertexData::getVertexBuffer() const { return vertexBuffer_.get(); }

IndexBuffer* NullVertexData::getIndexBuffer() const { return indexBuffer_.get(); }

void NullVertexData::nextTimestep() {}

std::pair<std::string, std::string> NullVertexData::toStringImpl() const {
  return std::make_pair("NullVertexData",
                        core::format("{}"
                                     "vertexBuffer = {},\n",
                                     "indexBuffer = {}\n", Base::toStringImpl().second,
                                     vertexBuffer_->toString(), indexBuffer_->toString()));
}

} // namespace render

} // namespace sequoia
