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
#include "sequoia-engine/Game/Drawable.h"
#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Game/SceneNode.h"
#include "sequoia-engine/Game/SceneNodeAlloc.h"
#include "sequoia-engine/Render/DrawCommand.h"

namespace sequoia {

namespace game {

Drawable::~Drawable() {}

Drawable::Drawable(SceneNode* node, const std::shared_ptr<Shape>& shape)
    : Base(node), active_(true), shape_(shape) {}

void Drawable::prepareDrawCommands(std::vector<render::DrawCommand>& drawCommands) {
  SEQUOIA_ASSERT(active_);

  math::mat4 modelMatrix = getNode()->getModelMatrix();
  shape_->forEach([this, &drawCommands, &modelMatrix](const std::shared_ptr<Mesh>& mesh,
                                                      const std::shared_ptr<Material>& material) {
    render::DrawCommand cmd(mesh->getVertexData(), modelMatrix);
    drawCommands.emplace_back(std::move(cmd));
  });
}

void Drawable::update(const SceneNodeUpdateEvent& event) {}

std::string Drawable::toString() const {
  return core::format("Drawable[\n"
                      "  active = {},\n"
                      "  shape = {},\n"
                      "]",
                      active_, shape_->toString());
}

std::shared_ptr<SceneNodeCapability> Drawable::clone(SceneNode* node) const {
  return scene::allocate_shared<Drawable>(node, shape_);
}

} // namespace game

} // namespace sequoia
