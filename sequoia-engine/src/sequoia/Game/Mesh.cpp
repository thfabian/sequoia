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

#include "sequoia/Core/Memory.h"
#include "sequoia/Game/Mesh.h"

namespace sequoia {

namespace game {

Mesh::Mesh(const std::string& name) : data_(nullptr), name_(name) {}

const math::AxisAlignedBox& Mesh::getAAB() const { return data_->AAB; }

std::size_t Mesh::getNumVertices() const { return data_->NumVertices; }

render::VertexArrayObject* Mesh::getVAO() const { return data_->VAO.get(); }

void Mesh::accept(render::VertexVisitor& visitor) const {
  visitor.setNumVertices(data_->NumVertices);
  visitor.setDataPtr(data_->DataPtr);
  data_->Layout->accept(visitor);
}

} // namespace game

} // namespace sequoia
