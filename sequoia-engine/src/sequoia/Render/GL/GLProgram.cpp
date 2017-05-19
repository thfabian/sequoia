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

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgram.h"
#include "sequoia/Render/GL/GLProgramManager.h"

namespace sequoia {

namespace render {

GLProgram::GLProgram(const std::set<Shader*>& shaders, GLProgramManager* manager)
    : Program(RK_OpenGL), status_(GLProgramStatus::Invalid), id_(0), shaders_(shaders),
      manager_(manager) {}

bool GLProgram::isValid() const { return (status_ == GLProgramStatus::Linked); }

const std::set<Shader*>& GLProgram::getShaders() const { return shaders_; }

void GLProgram::addShader(Shader* shader) {
  status_ = GLProgramStatus::Created;
  shaders_.insert(shader);
}

bool GLProgram::removeShader(Shader* shader) {
  status_ = GLProgramStatus::Created;
  return shaders_.erase(shader);
}

void GLProgram::setVertexAttribName(GLVertexAttrib attrib, const std::string& name) {
  manager_->setVertexAttribName(this, attrib, name);
}

const std::unordered_map<std::string, GLProgram::GLUniform>&
GLProgram::getUniformVariables() const {
  return uniformMap_;
}

GLProgramManager* GLProgram::getManager() const { return manager_; }

unsigned int GLProgram::getID() const { return id_; }

GLProgramStatus GLProgram::getStatus() const { return status_; }

void GLProgram::use() {
  if(!isValid())
    manager_->makeValid(this);
  glUseProgram(id_);
}

std::string GLProgram::toString() const { return manager_->getInfoLog(this); }

} // namespace render

} // namespace sequoia
