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

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/HostBuffer.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/D3D12/D3D12VertexData.h"

namespace sequoia {

namespace render {

D3D12VertexData::D3D12VertexData(const VertexDataParameter& param)
    : VertexData(RK_D3D12, param.DrawMode) {

  vertexBuffer_ = std::make_unique<D3D12VertexBuffer>(param.Layout);
  if(param.NumIndices > 0)
    indexBuffer_ = std::make_unique<D3D12IndexBuffer>(param.IndexType);

  // Allocate buffers
  vertexBuffer_->allocateVertices(param.NumVertices, param.VertexBufferUsageHint);
  if(indexBuffer_)
    indexBuffer_->allocateIndices(param.NumIndices);

  // Allocate shadow buffers
  if(param.UseVertexShadowBuffer)
    vertexBuffer_->setShadowBuffer(core::HostBuffer::create(vertexBuffer_->getNumBytes()));

  if(indexBuffer_ && param.UseIndexShadowBuffer)
    indexBuffer_->setShadowBuffer(core::HostBuffer::create(indexBuffer_->getNumBytes()));
}

D3D12VertexData::~D3D12VertexData() {}

VertexBuffer* D3D12VertexData::getVertexBuffer() const { return vertexBuffer_.get(); }

IndexBuffer* D3D12VertexData::getIndexBuffer() const { return indexBuffer_.get(); }

std::pair<std::string, std::string> D3D12VertexData::toStringImpl() const {
  return std::make_pair("D3D12VertexData",
                        core::format("{}"
                                     "vertexBuffer = {},\n"
                                     "indexBuffer = {}\n",
                                     Base::toStringImpl().second, vertexBuffer_->toString(),
                                     indexBuffer_ ? indexBuffer_->toString() : "d3d12"));
}

} // namespace render

} // namespace sequoia
