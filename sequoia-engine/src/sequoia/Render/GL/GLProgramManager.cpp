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
#include "sequoia/Render/GL/GLShader.h"
#include "sequoia/Render/GL/GLShaderManager.h"
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
  if(program->status_ == GLProgramStatus::Linked)
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

    glLinkProgram(program->id_);
    if(glGetError() != GL_NO_ERROR)
      SEQUOIA_THROW(RenderSystemException, "failed to link program");

    for(Shader* shader : program->getShaders()) {
      GLShader* glshader = dyn_cast<GLShader>(shader);
      glDetachShader(program->id_, glshader->getID());
    }

    LOG(DEBUG) << "Successfully linked program (ID=" << program->id_ << ")";
    program->status_ = GLProgramStatus::Linked;
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
