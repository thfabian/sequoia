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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Memory.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Game/Mesh.h"
#include "sequoia/Render/VertexVisitor.h"

namespace sequoia {

namespace game {

bool MeshParameter::operator==(const MeshParameter& other) const noexcept {
  return TexCoordInvertV == other.TexCoordInvertV;
}

std::string MeshParameter::toString() const {
  return core::format("MeshParameter[\n"
                      "  TexCoordInvertV = %s\n"
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
                      "  name = %s,\n"
                      "  data = %s\n"
                      "]",
                      name_, core::indent(data_->toString()));
}

} // namespace game

} // namespace sequoia
