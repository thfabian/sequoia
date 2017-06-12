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

#include "sequoia/Game/Mesh.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Memory.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/VertexVisitor.h"

namespace sequoia {

namespace game {

Mesh::Mesh(const std::string& name, const std::shared_ptr<render::VertexData>& data,
           bool isModifiable)
    : isModifiable_(isModifiable), data_(data), name_(name) {}

void Mesh::accept(render::VertexVisitor& visitor) const { data_->getLayout()->accept(visitor); }

void Mesh::dump() const { data_->dump(); }

std::string Mesh::toString() const {
  return core::format("Mesh[\n"
                      "  name = %s,\n"
                      "  data = %s\n"
                      "]",
                      name_, core::indent(data_->toString()));
}

void Mesh::logWarning() const {
  LOG(WARNING) << "Attempting to modify read-only mesh \"" << name_ << "\"";
}

} // namespace game

} // namespace sequoia
