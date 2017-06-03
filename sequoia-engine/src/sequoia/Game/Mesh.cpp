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
#include "sequoia/Game/Mesh.h"
#include "sequoia/Render/VertexVisitor.h"

namespace sequoia {

namespace game {

Mesh::Mesh(const std::string& name, const std::shared_ptr<render::VertexData>& data)
    : data_(data), name_(name) {}

void Mesh::accept(render::VertexVisitor& visitor) const { data_->getLayout()->accept(visitor); }

const std::string& Mesh::getName() const { return name_; }

std::string Mesh::toString() const {
  return core::format("Mesh[\n"
                      "  name = %s,\n"
                      "  data = %s\n"
                      "]",
                      name_, core::indent(data_->toString()));
}

} // namespace game

} // namespace sequoia
