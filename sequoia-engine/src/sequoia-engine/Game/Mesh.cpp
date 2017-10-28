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

#include "sequoia-engine/Game/Mesh.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Memory.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Render/VertexVisitor.h"

namespace sequoia {

namespace game {

bool MeshParameter::operator==(const MeshParameter& other) const noexcept {
  return TexCoordInvertV == other.TexCoordInvertV;
}

std::string MeshParameter::toString() const {
  return core::format("MeshParameter[\n"
                      "  TexCoordInvertV = {}\n"
                      "]",
                      TexCoordInvertV ? "true" : "false");
}

Mesh::Mesh(const std::string& name, const std::shared_ptr<render::VertexData>& data,
           bool isModifiable)
    : isModifiable_(isModifiable), data_(data), name_(name) {}

const math::AxisAlignedBox& Mesh::getAxisAlignedBox() const noexcept {
  return data_->getAxisAlignedBox();
}

void Mesh::setAxisAlignedBox(const math::AxisAlignedBox& bbox) { data_->setAxisAlignedBox(bbox); }

void Mesh::dump() const { data_->dump(); }

std::string Mesh::toString() const {
  return core::format("Mesh[\n"
                      "  name = {},\n"
                      "  data = {}\n"
                      "]",
                      name_, core::indent(data_->toString()));
}

} // namespace game

} // namespace sequoia
