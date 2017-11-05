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

#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Casting.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/StringRef.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/GL/GLFragmentData.h"
#include "sequoia-engine/Render/GL/GLProgramManager.h"
#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/GL/GLRenderer.h"
#include "sequoia-engine/Render/GL/GLShaderManager.h"
#include "sequoia-engine/Render/GL/GLVertexAttribute.h"
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

  Log::debug("Created program (ID={})", program->id_);

  Log::debug("Linking program (ID={}) ...", program->id_);

  for(const std::shared_ptr<Shader>& shader : program->getShaders()) {
    auto glshader = dyn_pointer_cast<GLShader>(shader);

    // TODO: if at some point we have multiple ressource threads, we need to wait here
    SEQUOIA_ASSERT_MSG(glshader->isValid(), "shader not valid");

    Log::debug("Attaching shader (ID={}) to program (ID={})", glshader->getID(), program->id_);
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

  Log::debug("Successfully linked program (ID={})", program->id_);
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
  Log::debug("Getting uniform variables of program (ID={}) ...", program->id_);

  program->uniformInfoMap_.clear();
  program->textureSamplers_.clear();
  program->allUniformVariablesSet_ = false;

  int numActiveUniforms = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
  Log::debug("Program has {} active uniform variables", numActiveUniforms);

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
        Log::warn("Failed to extract texture unit from uniform variable \"{}\": {}", name.get(), e.what());
        textureUnit = -1;
      }
    }

    Log::debug("Active uniform variable: name={}, type={}{}{}, location={}", name.get(), type, (rank != 1 ? core::format(", size={}", rank) : ""), (textureUnit != -1 ? core::format(", textureUnit={}", textureUnit) : ""), location);

    if(location != -1) {
      program->uniformInfoMap_.emplace(
          name.get(), GLProgram::GLUniformInfo{type, rank, location, false, textureUnit});

      if(textureUnit != -1) {
        auto ret = program->textureSamplers_.emplace(textureUnit, name.get());
        if(!ret.second)
          Log::warn("Texture sampler \"{}\" mapped to already existing texture unit '{}' which is mapped to \"{}\"", name.get(), textureUnit, ret.first->second);
      }
    }
  }

  Log::debug("Successfully got uniform variables of program (ID={})", program->id_);
}

void GLProgramManager::setVertexAttributes(GLProgram* program) const {
  Log::debug("Setting vertex attributes of program (ID={}) ...", program->id_);

  GLVertexAttribute::forEach([&program](unsigned int index, const char* name) {
    glBindAttribLocation(program->id_, index, name);
  });

  Log::debug("Successfully set vertex attributes of program (ID={})", program->id_);
}

bool GLProgramManager::checkVertexAttributes(GLProgram* program) const {
  Log::debug("Checking vertex attributes of program (ID={}) ...", program->id_);

  int numActiveAttrs = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_ATTRIBUTES, &numActiveAttrs);
  Log::debug("Program has {} active vertex attributes", numActiveAttrs);

  int activeAttrMaxLength = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &activeAttrMaxLength);
  auto name = std::make_unique<char[]>(activeAttrMaxLength);

  for(unsigned int index = 0; index < numActiveAttrs; ++index) {
    GLenum type;
    GLint rank, length;
    glGetActiveAttrib(program->id_, index, activeAttrMaxLength, &length, &rank, &type, name.get());
    GLint location = glGetAttribLocation(program->id_, name.get());

    Log::debug("Active vertex attribute: name={}, type={}, location={}", name.get(), type, location);

    // We don't check attributes which start with `frag_`
    if(!StringRef(name.get()).startswith("frag_") && !GLVertexAttribute::isValid(name.get())) {
      SEQUOIA_THROW(RenderSystemException, "invalid vertex attribute '{}'", name.get());
    }
  }

  Log::debug("Successfully checked vertex attributes of program (ID={})", program->id_);
  return true;
}

void GLProgramManager::setFragmentData(GLProgram* program) const {
  Log::debug("Setting fragment data of program (ID={}) ...", program->id_);

  GLFragmentData::forEach([&program](unsigned int index, const char* name) {
    glBindFragDataLocation(program->id_, index, name);
  });

  Log::debug("Successfully set fragment data of program (ID={})", program->id_);
}

bool GLProgramManager::checkFragmentData(GLProgram* program) const {
  if(getGLRenderer().isExtensionSupported(GLextension::GL_ARB_program_interface_query)) {
    Log::debug("Checking fragment data of program (ID={}) ...", program->id_);

    int numFragData = 0;
    glGetProgramInterfaceiv(program->id_, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &numFragData);
    Log::debug("Program has {} ouput fragment data", numFragData);

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

      Log::debug("Output fragment data: name={}, location={}", name, location);

      // We only check variables which are explicitly tagged as `out_*`
      if(StringRef(name).startswith("out_") && !GLFragmentData::isValid(name.c_str()))
        SEQUOIA_THROW(RenderSystemException, "invalid output fragment data '{}'", name);
    }

    Log::debug("Successfully checked fragment data of program (ID={})", program->id_);
  }
  return true;
}

} // namespace render

} // namespace sequoia
