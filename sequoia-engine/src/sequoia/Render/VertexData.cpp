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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/Memory.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/VertexArrayObject.h"
#include "sequoia/Render/VertexData.h"
#include "sequoia/Render/VertexVisitor.h"
#include <iostream>

namespace sequoia {

namespace render {

VertexData::VertexData(const VertexLayout* layout, std::size_t numVertices, std::size_t numIndices)
    : numVertices_(numVertices), numIndices_(numIndices), layout_(layout) {

  verticesPtr_ = memory::aligned_alloc(numVertices_ * layout_->SizeOf);
  if(!verticesPtr_)
    SEQUOIA_THROW(RenderSystemException, "failed to allocate vertex data: out of memory");

  if(numIndices_ > 0) {
    indicesPtr_ = (IndicesType*)memory::aligned_alloc(numIndices_ * sizeof(IndicesType));

    if(!indicesPtr_)
      SEQUOIA_THROW(RenderSystemException, "failed to allocate index data: out of memory");
  } else
    indicesPtr_ = nullptr;
}

VertexData::~VertexData() {
  vao_.reset();

  memory::aligned_free(verticesPtr_);
  if(indicesPtr_)
    memory::aligned_free(indicesPtr_);

  verticesPtr_ = nullptr;
  numVertices_ = 0;
  indicesPtr_ = nullptr;
  numIndices_ = 0;
  layout_ = nullptr;
  aab_.clear();
}

void VertexData::setVertexArrayObject(std::unique_ptr<VertexArrayObject> vao,
                                      VertexArrayObject::BufferUsageKind usage) {
  vao_ = std::move(vao);
  vao_->attachVertexData(this, usage);
}

void VertexData::accept(VertexVisitor& visitor) const {
  visitor.setNumVertices(numVertices_);
  visitor.setVerticesPtr(verticesPtr_);
  layout_->accept(visitor);
}

void VertexData::dumpVertexData() const {
  VertexVisitorStringifier visitor;
  accept(visitor);
  std::cout << visitor.toString() << std::endl;
}

std::string VertexData::toString() const {
  return core::format("VertexData[\n"
                      "  numVertices = %s,\n"
                      "  vao = %s,\n"
                      "  numIndices = %s,\n"
                      "  layout = %s,\n"
                      "  aab = %s\n"
                      "]",
                      numVertices_, core::indent(vao_->toString()), numIndices_,
                      core::indent(layout_->toString()), core::indent(aab_.toString()));
}

} // namespace render

} // namespace sequoia
