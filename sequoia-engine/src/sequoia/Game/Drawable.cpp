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
    : Base(node), active_(true), drawCommand_(SceneNodeAlloc::create<render::DrawCommand>()) {
  setMesh(mesh);
  setProgram(program ? program : Game::getSingleton().getDefaultProgram().get());
}

Drawable::Drawable(SceneNode* node, const std::shared_ptr<Mesh>& mesh,
                   const std::shared_ptr<render::Program>& program)
    : Drawable(node, mesh, program.get()) {}

void Drawable::setMesh(const std::shared_ptr<Mesh>& mesh) {
  mesh_ = mesh;
  drawCommand_->setVertexArrayObject(mesh_->getVertexArrayObject());
}

render::RenderState& Drawable::getRenderState() { return drawCommand_->getRenderState(); }
const render::RenderState& Drawable::getRenderState() const {
  return drawCommand_->getRenderState();
}

void Drawable::setProgram(render::Program* program) { drawCommand_->setProgram(program); }

render::DrawCommand* Drawable::prepareDrawCommand() {
  SEQUOIA_ASSERT(active_);
  SEQUOIA_ASSERT_MSG(mesh_, "no Mesh set in DrawCommand");
  SEQUOIA_ASSERT_MSG(drawCommand_->getVertexArrayObject(),
                     "no VertexArrayObject set in DrawCommand");
  SEQUOIA_ASSERT_MSG(drawCommand_->getProgram(), "no Program set in DrawCommand");

  // Copy ModelMatrix to the draw command
  drawCommand_->setModelMatrix(getNode()->getModelMatrix());

  return drawCommand_.get();
}

void Drawable::update(const SceneNodeUpdateEvent& event) {}

std::shared_ptr<SceneNodeCapability> Drawable::clone(SceneNode* node) const {
  return SceneNodeAlloc::create<Drawable>(node, mesh_, drawCommand_->getProgram());
}

void Drawable::setUniformVariableImpl(const std::string& name, UniformVariable variable) {
  drawCommand_->setUniformVariable(name, variable);
}

std::string Drawable::toString() const {
  return core::format("Drawable[\n"
                      "  drawCommand = %s,\n"
                      "  mesh = %s,\n"
                      "]",
                      drawCommand_->toString(), mesh_ ? mesh_->toString() : "null");
}

} // namespace game

} // namespace sequoia
