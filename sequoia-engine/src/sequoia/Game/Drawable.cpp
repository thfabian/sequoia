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

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Game/Drawable.h"
#include "sequoia/Game/Mesh.h"
#include "sequoia/Game/SceneGraph.h"
#include "sequoia/Render/DrawCommand.h"

namespace sequoia {

namespace game {

Drawable::Drawable(const std::string& name, SceneNodeKind kind)
    : Base(name, kind), active_(true), drawCommand_(std::make_shared<render::DrawCommand>()),
      mesh_(nullptr) {}

Drawable::Drawable(const Drawable& other)
    : Base(other), active_(other.active_), drawCommand_(other.drawCommand_), mesh_(other.mesh_) {}

Drawable::~Drawable() {}

void Drawable::setMesh(const std::shared_ptr<Mesh>& mesh) {
  mesh_ = mesh;
  drawCommand_->setVertexArrayObject(mesh_->getVertexArrayObject());
}

void Drawable::setProgram(const std::shared_ptr<render::Program>& program) {
  drawCommand_->setProgram(program.get());
}

render::DrawCommand* Drawable::prepareDrawCommand() {
  SEQUOIA_ASSERT(active_);
  SEQUOIA_ASSERT_MSG(mesh_, "no Mesh set in DrawCommand");
  SEQUOIA_ASSERT_MSG(drawCommand_->getVertexArrayObject(),
                     "no VertexArrayObject set in DrawCommand");
  SEQUOIA_ASSERT_MSG(drawCommand_->getProgram(), "no Program set in DrawCommand");

  // Copy ModelMatrix to the draw command
  drawCommand_->setModelMatrix(getModelMatrix());

  return drawCommand_.get();
}

std::shared_ptr<SceneNode> Drawable::clone() { return SceneGraph::create<Drawable>(*this); }

std::pair<std::string, std::string> Drawable::toStringImpl() const {
  return std::make_pair("Drawable", core::format("%s"
                                                 "drawCommand = %s,\n"
                                                 "mesh = %s,\n",
                                                 Base::toStringImpl().second,
                                                 drawCommand_ ? drawCommand_->toString() : "null",
                                                 mesh_ ? mesh_->toString() : "null"));
}

} // namespace game

} // namespace sequoia
