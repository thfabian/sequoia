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

#include "sequoia-engine/Game/Drawable.h"
#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Game/Game.h"
#include "sequoia-engine/Game/Mesh.h"
#include "sequoia-engine/Game/SceneNode.h"
#include "sequoia-engine/Game/SceneNodeAlloc.h"
#include "sequoia-engine/Render/DrawCommand.h"

namespace sequoia {

namespace game {

Drawable::~Drawable() {}

Drawable::Drawable(SceneNode* node, const std::shared_ptr<Mesh>& mesh)
    : Base(node), active_(true), drawCommand_() {
  setMesh(mesh);
}

void Drawable::setMesh(const std::shared_ptr<Mesh>& mesh) {
  mesh_ = mesh;
  drawCommand_.get().setVertexData(mesh_->getVertexData());
}

void Drawable::setTexture(int textureUnit, render::Texture* texture) noexcept {
  drawCommand_.get().setTexture(textureUnit, texture);
}

void Drawable::setUniform(const std::string& name, UniformVariable variable) {
  drawCommand_.get().setUniformVariable(name, variable);
}

render::DrawCommand* Drawable::prepareDrawCommand() {
  SEQUOIA_ASSERT(active_);

  // Set the new model matrix
  drawCommand_.get().setModelMatrix(getNode()->getModelMatrix());

  // Advance the vertex buffers to the next time-step

  // Progress to the next time-step with the Drawcommand. Note that this copies the current
  // draw command (i.e cmd) into the new one so that subsequent calls will not corrupt the
  // render-state.
  render::DrawCommand* cmd = &drawCommand_.get();
  drawCommand_.nextTimestep();
  return cmd;
}

void Drawable::update(const SceneNodeUpdateEvent& event) {}

std::string Drawable::toString() const {
  return core::format("Drawable[\n"
                      "  drawCommand = {},\n"
                      "  mesh = {},\n"
                      "]",
                      drawCommand_.get().toString(), mesh_ ? mesh_->toString() : "null");
}

std::shared_ptr<SceneNodeCapability> Drawable::clone(SceneNode* node) const {
  return scene::allocate_shared<Drawable>(node, mesh_);
}

} // namespace game

} // namespace sequoia
