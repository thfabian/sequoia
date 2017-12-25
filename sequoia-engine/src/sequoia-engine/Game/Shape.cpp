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

#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Game/Shape.h"

namespace sequoia {

namespace game {

Shape::Shape(const std::string& name, std::vector<std::shared_ptr<Mesh>> meshes,
             std::vector<std::shared_ptr<Material>> materials)
    : name_(name), meshes_(std::move(meshes)), materials_(std::move(materials)) {
  SEQUOIA_ASSERT(meshes_.size() == materials_.size());
}

void Shape::add(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material) {
  meshes_.emplace_back(mesh);
  materials_.emplace_back(material);
}

void Shape::forEach(
    std::function<void(const std::shared_ptr<Mesh>&, const std::shared_ptr<Material>&)> func)
    const {
  for(int i = 0; i < materials_.size(); ++i)
    func(meshes_[i], materials_[i]);
}

std::string Shape::toString() const {
  return core::format(
      "Shape[\n"
      "  name = {},\n"
      "  meshes = {},\n"
      "  materials = {}\n"
      "]",
      name_, core::toStringRange(meshes_, [](const auto& meshPtr) { return meshPtr->toString(); }),
      core::toStringRange(materials_, [](const auto& matPtr) { return matPtr->toString(); }));
}

} // namespace game

} // namespace sequoia
