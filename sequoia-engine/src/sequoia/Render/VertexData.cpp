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

#include "sequoia/Render/VertexData.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Memory.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/VertexArrayObject.h"
#include "sequoia/Render/VertexVisitor.h"
#include <iostream>

namespace sequoia {

namespace render {

static const char* drawModeToString(VertexData::DrawModeKind mode) {
  switch(mode) {
  case VertexData::DrawModeKind::DM_Triangles:
    return "Triangles";
  default:
    sequoia_unreachable("invalid DrawModeKind");
  }
}

VertexData::VertexData(const VertexLayout* layout, DrawModeKind drawMode, std::size_t numVertices,
                       std::size_t numIndices, bool shadowBuffer)
    : numVertices_(numVertices), numIndices_(numIndices), layout_(layout), drawMode_(drawMode),
      shadowBuffer_(shadowBuffer) {

  verticesPtr_ = memory::aligned_alloc(numVertices_ * layout_->SizeOf);
  if(!verticesPtr_)
    SEQUOIA_THROW(RenderSystemException, "failed to allocate vertex data: out of memory");

  if(numIndices_ > 0) {
    indicesPtr_ = (VertexIndexType*)memory::aligned_alloc(numIndices_ * sizeof(VertexIndexType));

    if(!indicesPtr_)
      SEQUOIA_THROW(RenderSystemException, "failed to allocate index data: out of memory");
  } else
    indicesPtr_ = nullptr;
}

VertexData::~VertexData() {
  vao_.release();

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

void VertexData::accept(VertexVisitor& visitor, bool write) const {

  //  shadowBuffer_

  visitor.setNumVertices(numVertices_);
  visitor.setVerticesPtr(verticesPtr_);
  layout_->accept(visitor);
}

void VertexData::dump() const {
  std::cout << "Vertices = [\n";

  // Print vertices
  VertexVisitorStringifier visitor;
  acceptReadVisitor(visitor);
  std::cout << "  " << core::indent(visitor.toString()) << "]";

  // Print indices
  if(numIndices_ > 0) {
    std::cout << ",\nIndices = [\n";
    for(std::size_t i = 0; i < numIndices_; ++i)
      std::cout << "  " << indicesPtr_[i] << ((i == numIndices_ - 1) ? "" : ",") << "\n";
    std::cout << "]\n";
  } else
    std::cout << "\n";
}

std::string VertexData::toString() const {
  return core::format("VertexData[\n"
                      "  numVertices = %s,\n"
                      "  vao = %s,\n"
                      "  numIndices = %s,\n"
                      "  layout = %s,\n"
                      "  drawMode = %s,\n"
                      "  aab = %s,\n"
                      "  shadowBuffer = %s\n"
                      "]",
                      numVertices_, vao_ ? core::indent(vao_->toString()) : "null", numIndices_,
                      core::indent(layout_->toString()), drawModeToString(drawMode_),
                      core::indent(aab_.toString()), shadowBuffer_ ? "true" : "false");
}

} // namespace render

} // namespace sequoia
