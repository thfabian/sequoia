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

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/StringRef.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLFragmentData.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Render/GL/GLVertexAttribute.h"
#include <algorithm>
#include <boost/functional/hash.hpp>

namespace sequoia {

namespace render {

GLProgramManager::~GLProgramManager() {}

void GLProgramManager::makeValid(GLProgram* program) {
  SEQUOIA_ASSERT_MSG(!program->isValid(), "program already initialized");

  program->id_ = glCreateProgram();

  if(program->id_ == 0)
    SEQUOIA_THROW(RenderSystemException, "failed to create program");

  LOG(DEBUG) << "Created program (ID=" << program->id_ << ")";

  LOG(DEBUG) << "Linking program (ID=" << program->id_ << ") ...";

  for(const std::shared_ptr<Shader>& shader : program->getShaders()) {
    auto glshader = dyn_pointer_cast<GLShader>(shader);

    // TODO: if at some point we have multiple ressource threads, we need to wait here
    SEQUOIA_ASSERT_MSG(glshader->isValid(), "shader not valid");

    LOG(DEBUG) << "Attaching shader (ID=" << glshader->getID()
               << ") to program (ID=" << program->id_ << ")";
    glAttachShader(program->id_, glshader->getID());
  }

  // Set location of the vertex attributes and fragment data
  setVertexAttributes(program);
  setFragmentData(program);

  // Link the program
  glLinkProgram(program->id_);
  if(glGetError() != GL_NO_ERROR)
    SEQUOIA_THROW(RenderSystemException, "failed to link program");

  for(const auto& shader : program->getShaders()) {
    GLShader* glshader = dyn_cast<GLShader>(shader.get());
    glDetachShader(program->id_, glshader->getID());
  }

  // Get the uniform variables
  getUniforms(program);

  // Check all vertex attributes and fragment data
  SEQUOIA_ASSERT(checkVertexAttributes(program));
  SEQUOIA_ASSERT(checkFragmentData(program));

  LOG(DEBUG) << "Successfully linked program (ID=" << program->id_ << ")";
}

std::shared_ptr<GLProgram>
GLProgramManager::create(const std::set<std::shared_ptr<Shader>>& shaders) {
  SEQUOIA_LOCK_GUARD(mutex_);

  std::size_t hash = GLProgramManager::hash(shaders);
  auto it = shaderSetLookupMap_.find(hash);

  if(it != shaderSetLookupMap_.end())
    return programList_[it->second];

  programList_.emplace_back(std::make_shared<GLProgram>(shaders));
  shaderSetLookupMap_[hash] = programList_.size() - 1;
  return programList_.back();
}

void GLProgramManager::remove(const std::shared_ptr<GLProgram>& program) noexcept {
  SEQUOIA_LOCK_GUARD(mutex_);

  programList_.erase(std::remove(programList_.begin(), programList_.end(), program),
                     programList_.end());
  shaderSetLookupMap_.erase(GLProgramManager::hash(program->getShaders()));
}

std::size_t GLProgramManager::hash(const std::set<std::shared_ptr<Shader>>& shaders) noexcept {
  std::size_t seed = 0;
  std::for_each(shaders.begin(), shaders.end(), [&seed](const std::shared_ptr<Shader>& shader) {
    boost::hash_combine(seed, std::hash<Shader*>()(shader.get()));
  });
  return seed;
}

void GLProgramManager::getUniforms(GLProgram* program) const {
  LOG(DEBUG) << "Getting uniform variables of program (ID=" << program->id_ << ") ...";

  program->uniformInfoMap_.clear();
  program->textureSamplers_.clear();
  program->allUniformVariablesSet_ = false;
  program->reportedWarningForInvalidUniformVariable_.clear();

  int numActiveUniforms = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
  LOG(DEBUG) << "Program has " << numActiveUniforms << " active uniform variables";

  int activeUniformMaxLength = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &activeUniformMaxLength);
  auto name = std::make_unique<char[]>(activeUniformMaxLength);

  for(int index = 0; index < numActiveUniforms; ++index) {
    GLenum type;
    GLint rank, length;
    glGetActiveUniform(program->id_, index, activeUniformMaxLength, &length, &rank, &type,
                       name.get());

    GLint location = glGetUniformLocation(program->id_, name.get());

    // Does the variable correspond to a texture sampler?
    int textureUnit = -1;
    StringRef nameRef(name.get());
    if(nameRef.startswith("tex")) {
      // extract 0 from `tex0_XXX`
      std::string textureUnitStr = nameRef.substr(3, nameRef.find_first_of('_', 3)).str();

      try {
        textureUnit = std::stoi(textureUnitStr.c_str());
      } catch(std::invalid_argument& e) {
        LOG(WARNING) << "Failed to extract texture unit from uniform variable \"" << name.get()
                     << "\": " << e.what();
        textureUnit = -1;
      }
    }

    LOG(DEBUG) << "Active uniform variable: name=" << name.get() << ", type=" << type
               << (rank != 1 ? core::format(", size=%i", rank) : "")
               << (textureUnit != -1 ? core::format(", textureUnit=%i", textureUnit) : "")
               << ", location=" << location;

    if(location != -1) {
      program->uniformInfoMap_.emplace(
          name.get(), GLProgram::GLUniformInfo{type, rank, location, false, textureUnit});

      if(textureUnit != -1) {
        auto ret = program->textureSamplers_.emplace(textureUnit, name.get());
        if(!ret.second)
          LOG(WARNING) << "Texture sampler \"" << name.get()
                       << "\" mapped to already existing texture unit '" << textureUnit
                       << "' which is mapped to \"" << ret.first->second << "\"";
      }
    }
  }

  LOG(DEBUG) << "Successfully got uniform variables of program (ID=" << program->id_ << ")";
}

void GLProgramManager::setVertexAttributes(GLProgram* program) const {
  LOG(DEBUG) << "Setting vertex attributes of program (ID=" << program->id_ << ") ...";

  GLVertexAttribute::forEach([&program](unsigned int index, const char* name) {
    glBindAttribLocation(program->id_, index, name);
  });

  LOG(DEBUG) << "Successfully set vertex attributes of program (ID=" << program->id_ << ")";
}

bool GLProgramManager::checkVertexAttributes(GLProgram* program) const {
  LOG(DEBUG) << "Checking vertex attributes of program (ID=" << program->id_ << ") ...";

  int numActiveAttrs = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_ATTRIBUTES, &numActiveAttrs);
  LOG(DEBUG) << "Program has " << numActiveAttrs << " active vertex attributes";

  int activeAttrMaxLength = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &activeAttrMaxLength);
  auto name = std::make_unique<char[]>(activeAttrMaxLength);

  for(unsigned int index = 0; index < numActiveAttrs; ++index) {
    GLenum type;
    GLint rank, length;
    glGetActiveAttrib(program->id_, index, activeAttrMaxLength, &length, &rank, &type, name.get());
    GLint location = glGetAttribLocation(program->id_, name.get());

    LOG(DEBUG) << "Active vertex attribute: name=" << name.get() << ", type=" << type
               << ", location=" << location;

    // We don't check attributes which start with `frag_`
    if(!StringRef(name.get()).startswith("frag_") && !GLVertexAttribute::isValid(name.get())) {
      SEQUOIA_THROW(RenderSystemException, "invalid vertex attribute '%s'", name.get());
    }
  }

  LOG(DEBUG) << "Successfully checked vertex attributes of program (ID=" << program->id_ << ")";
  return true;
}

void GLProgramManager::setFragmentData(GLProgram* program) const {
  LOG(DEBUG) << "Setting fragment data of program (ID=" << program->id_ << ") ...";

  GLFragmentData::forEach([&program](unsigned int index, const char* name) {
    glBindFragDataLocation(program->id_, index, name);
  });

  LOG(DEBUG) << "Successfully set fragment data of program (ID=" << program->id_ << ")";
}

bool GLProgramManager::checkFragmentData(GLProgram* program) const {
  if(getGLRenderer().isExtensionSupported(GLextension::GL_ARB_program_interface_query)) {
    LOG(DEBUG) << "Checking fragment data of program (ID=" << program->id_ << ") ...";

    int numFragData = 0;
    glGetProgramInterfaceiv(program->id_, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &numFragData);
    LOG(DEBUG) << "Program has " << numFragData << " ouput fragment data";

    std::array<GLenum, 2> properties = {{GL_NAME_LENGTH, GL_LOCATION}};
    std::array<GLint, 2> values;

    for(unsigned int index = 0; index < numFragData; ++index) {
      glGetProgramResourceiv(program->id_, GL_PROGRAM_OUTPUT, index, properties.size(),
                             properties.data(), values.size(), nullptr, values.data());

      std::vector<char> nameData(values[0]);
      glGetProgramResourceName(program->id_, GL_PROGRAM_OUTPUT, index, nameData.size(), NULL,
                               nameData.data());
      std::string name(nameData.begin(), nameData.end() - 1);
      GLint location = values[1];

      LOG(DEBUG) << "Output fragment data: name=" << name << ", location=" << location;

      // We only check variables which are explicitly tagged as `out_*`
      if(StringRef(name).startswith("out_") && !GLFragmentData::isValid(name.c_str()))
        SEQUOIA_THROW(RenderSystemException, "invalid output fragment data '%s'", name);
    }

    LOG(DEBUG) << "Successfully checked fragment data of program (ID=" << program->id_ << ")";
  }
  return true;
}

} // namespace render

} // namespace sequoia
