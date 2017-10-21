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

#include "sequoia/Game/Drawable.h"
#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Game/Game.h"
#include "sequoia/Game/Mesh.h"
#include "sequoia/Game/SceneNode.h"
#include "sequoia/Game/SceneNodeAlloc.h"
#include "sequoia/Render/DrawCommand.h"

namespace sequoia {

namespace game {

Drawable::~Drawable() {}

Drawable::Drawable(SceneNode* node, const std::shared_ptr<Mesh>& mesh, render::Program* program)
    : Base(node), active_(true), drawCommand_() {
  setMesh(mesh);
  setProgram(program ? program : Game::getSingleton().getDefaultProgram().get());
}

Drawable::Drawable(SceneNode* node, const std::shared_ptr<Mesh>& mesh,
                   const std::shared_ptr<render::Program>& program)
    : Drawable(node, mesh, program.get()) {}

void Drawable::setMesh(const std::shared_ptr<Mesh>& mesh) {
  mesh_ = mesh;
  drawCommand_.get().setVertexData(mesh_->getVertexData());
}

void Drawable::setTexture(int textureUnit, render::Texture* texture) noexcept {
  drawCommand_.get().setTexture(textureUnit, texture);
}

render::RenderState& Drawable::getRenderState() { return drawCommand_.get().getRenderState(); }
const render::RenderState& Drawable::getRenderState() const {
  return drawCommand_.get().getRenderState();
}

void Drawable::setProgram(render::Program* program) noexcept {
  drawCommand_.get().setProgram(program);
}

render::DrawCommand* Drawable::prepareDrawCommand() {
  SEQUOIA_ASSERT(active_);
  SEQUOIA_ASSERT_MSG(drawCommand_.get().getProgram(), "no Program set");

  // Set the new model matrix
  drawCommand_.get().setModelMatrix(getNode()->getModelMatrix());

  // Advance the vertex buffers to the next time-step

  // Progress to the next time-step with the Drawcommand. Note that this copies the current
  // draw command (i.e cmd) into the new one so that subsequent calls will not corrupt the
  // render-state.
  render::DrawCommand* cmd = &drawCommand_.get();
  drawCommand_.nextTimestep();
  drawCommand_.get().getVertexData()->nextTimestep();
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
  return scene::allocate_shared<Drawable>(node, mesh_, drawCommand_.get().getProgram());
}

void Drawable::setUniformVariableImpl(const std::string& name, UniformVariable variable) {
  drawCommand_.get().setUniformVariable(name, variable);
}

} // namespace game

} // namespace sequoia
