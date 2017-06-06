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

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Game/Mesh.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Game/SceneNodeDrawable.h"
#include "sequoia/Render/DrawCommand.h"

namespace sequoia {

namespace game {

SceneNodeDrawable::SceneNodeDrawable(const std::string& name) : Base(name, SK_SceneNodeDrawable) {}

SceneNodeDrawable::SceneNodeDrawable(const SceneNodeDrawable& other)
    : Base(other), drawCommand_(other.drawCommand_), mesh_(other.mesh_) {}

SceneNodeDrawable::~SceneNodeDrawable() {}

void SceneNodeDrawable::setMesh(const std::shared_ptr<Mesh>& mesh) {
  mesh_ = mesh;
  if(hasDrawCommand())
    drawCommand_->setVertexArrayObject(mesh_->getVertexArrayObject());
}

void SceneNodeDrawable::setDrawCommand(const std::shared_ptr<render::DrawCommand>& drawCommand) {
  drawCommand_ = drawCommand;
  if(hasMesh())
    drawCommand_->setVertexArrayObject(mesh_->getVertexArrayObject());
}

render::DrawCommand* SceneNodeDrawable::prepareDrawCommand() {
  SEQUOIA_ASSERT_MSG(drawCommand_, "no DrawCommand set");
  SEQUOIA_ASSERT_MSG(mesh_, "no Mesh set in DrawCommand");
  SEQUOIA_ASSERT_MSG(drawCommand_->getProgram(), "no Program set in DrawCommand");

  // Set VAO
  drawCommand_->setVertexArrayObject(mesh_->getVertexArrayObject());

  // Set ModelMatrix
  drawCommand_->setModelMatrix(getModelMatrix());

  return drawCommand_.get();
}

std::shared_ptr<SceneNode> SceneNodeDrawable::clone() {
  return SceneGraph::create<SceneNodeDrawable>(*this);
}

std::pair<std::string, std::string> SceneNodeDrawable::toStringImpl() const {
  return std::make_pair("SceneNodeDrawable",
                        core::format("%s"
                                     "drawCommand = %s,\n"
                                     "mesh = %s,\n",
                                     Base::toStringImpl().second,
                                     drawCommand_ ? drawCommand_->toString() : "null",
                                     mesh_ ? mesh_->toString() : "null"));
}

} // namespace game

} // namespace sequoia