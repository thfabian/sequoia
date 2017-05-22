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

#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include <algorithm>
#include <boost/functional/hash.hpp>

namespace sequoia {

namespace render {

std::size_t GLProgramManager::hash(const std::set<Shader*>& shaders) noexcept {
  std::size_t seed = 0;
  std::for_each(shaders.begin(), shaders.end(), [&seed](Shader* shader) {
    boost::hash_combine(seed, std::hash<Shader*>()(shader));
  });
  return seed;
}

GLProgramManager::~GLProgramManager() {
  for(auto& programPtr : programList_)
    destroy(programPtr.get());
}

void GLProgramManager::make(GLProgram* program, GLProgramStatus requestedStatus) {
  if(program->status_ == GLProgramStatus::Linked && requestedStatus != GLProgramStatus::Invalid)
    return;

  if(requestedStatus == GLProgramStatus::Invalid)
    destroy(program);

  if(program->status_ == GLProgramStatus::Invalid) {
    program->id_ = glCreateProgram();

    if(program->id_ == 0)
      SEQUOIA_THROW(RenderSystemException, "failed to create program");

    LOG(DEBUG) << "Created program (ID=" << program->id_ << ")";
    program->status_ = GLProgramStatus::Created;
  }

  if(requestedStatus == GLProgramStatus::Created)
    return;

  if(program->status_ == GLProgramStatus::Created) {
    LOG(DEBUG) << "Linking program (ID=" << program->id_ << ") ...";

    for(Shader* shader : program->getShaders()) {
      GLShader* glshader = dyn_cast<GLShader>(shader);

      // Make sure shader is valid
      glshader->getManager()->makeValid(glshader);

      LOG(DEBUG) << "Attaching shader (ID=" << glshader->getID()
                 << ") to program (ID=" << program->id_ << ")";
      glAttachShader(program->id_, glshader->getID());
    }

    // Set location of the vertex attributes
    setAttributes(program);

    // Link the program
    glLinkProgram(program->id_);
    if(glGetError() != GL_NO_ERROR)
      SEQUOIA_THROW(RenderSystemException, "failed to link program");
    program->status_ = GLProgramStatus::Linked;

    for(Shader* shader : program->getShaders()) {
      GLShader* glshader = dyn_cast<GLShader>(shader);
      glDetachShader(program->id_, glshader->getID());
    }

    // Get the uniform variables
    getUniforms(program);

    // Check all vertex attributes have been set
    checkAttributes(program);

    LOG(DEBUG) << "Successfully linked program (ID=" << program->id_ << ")";
  }
}

void GLProgramManager::destroy(GLProgram* program) {
  if(program->status_ == GLProgramStatus::Invalid)
    return;

  LOG(DEBUG) << "Deleting program (ID=" << program->id_ << ")";
  glDeleteProgram(program->id_);
  program->id_ = 0;
  program->status_ = GLProgramStatus::Invalid;
}

void GLProgramManager::getUniforms(GLProgram* program) const {
  LOG(DEBUG) << "Getting uniform variables of program (ID=" << program->id_ << ")";
  SEQUOIA_ASSERT(program->status_ == GLProgramStatus::Linked);

  program->uniformInfoMap_.clear();
  program->allUniformVariablesSet_ = false;

  int numActiveUniforms = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
  LOG(DEBUG) << "Program has " << numActiveUniforms << " active uniform variables";

  int activeUniformMaxLength = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &activeUniformMaxLength);
  auto name = std::make_unique<char[]>(activeUniformMaxLength);

  for(int index = 0; index < numActiveUniforms; ++index) {
    GLenum type;
    GLint size, length;
    glGetActiveUniform(program->id_, index, activeUniformMaxLength, &length, &size, &type,
                       name.get());
    GLint location = glGetUniformLocation(program->id_, name.get());

    LOG(DEBUG) << "Active uniform variable: name=" << name.get() << ", type=" << type
               << ", location=" << location;

    if(location != -1)
      program->uniformInfoMap_.emplace(name.get(),
                                       GLProgram::GLUniformInfo{type, size, location, false});
  }

  LOG(DEBUG) << "Successfully got uniform variables of program (ID=" << program->id_ << ")";
}

void GLProgramManager::setAttributes(GLProgram* program) const {
  LOG(DEBUG) << "Setting vertex attributes of program (ID=" << program->id_ << ")";

  GLVertexAttribute::forEach([&program](unsigned int index, const char* name) {
    glBindAttribLocation(program->id_, index, name);
  });

  LOG(DEBUG) << "Successfully set vertex attributes of program (ID=" << program->id_ << ")";
}

void GLProgramManager::checkAttributes(GLProgram* program) const {
  LOG(DEBUG) << "Checking vertex attributes of program (ID=" << program->id_ << ")";
  SEQUOIA_ASSERT(program->status_ == GLProgramStatus::Linked);

  int numActiveAttrs = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_ATTRIBUTES, &numActiveAttrs);
  LOG(DEBUG) << "Program has " << numActiveAttrs << " active vertex attributes";

  int activeAttrMaxLength = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &activeAttrMaxLength);
  auto name = std::make_unique<char[]>(activeAttrMaxLength);

  for(unsigned int index = 0; index < numActiveAttrs; ++index) {
    GLenum type;
    GLint size, length;
    glGetActiveAttrib(program->id_, index, activeAttrMaxLength, &length, &size, &type, name.get());
    GLint location = glGetAttribLocation(program->id_, name.get());

    LOG(DEBUG) << "Active vertex attribute: name=" << name.get() << ", type=" << type
               << ", location=" << location;

    if(!GLVertexAttribute::isValid(name.get()))
      SEQUOIA_THROW(RenderSystemException, "invalid vertex attribute '%s' ", name.get());
  }

  LOG(DEBUG) << "Successfully checked vertex attributes of program (ID=" << program->id_ << ")";
}

GLProgram* GLProgramManager::create(const std::set<Shader*>& shaders,
                                    GLProgramStatus requestedStatus) {
  GLProgram* program = nullptr;

  std::size_t hash = GLProgramManager::hash(shaders);
  auto it = shaderSetLookupMap_.find(hash);

  if(it != shaderSetLookupMap_.end())
    program = programList_[it->second].get();
  else {
    programList_.emplace_back(std::make_unique<GLProgram>(shaders, this));
    shaderSetLookupMap_[hash] = programList_.size() - 1;
    program = programList_.back().get();
  }

  make(program, requestedStatus);
  return program;
}

} // namespace render

} // namespace sequoia
