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

#include "sequoia/Render/GL/GLProgram.h"
#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/STLExtras.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLShader.h"
#include <sstream>

namespace sequoia {

namespace render {

static const char* statusToString(GLProgramStatus status) {
  switch(status) {
  case GLProgramStatus::Invalid:
    return "Invalid";
  case GLProgramStatus::Created:
    return "Created";
  case GLProgramStatus::Linked:
    return "Linked";
  default:
    sequoia_unreachable("invalid GLProgramStatus");
  }
}

const std::string GLProgram::EmptyString;

GLProgram::GLProgram(const std::set<std::shared_ptr<Shader>>& shaders, GLProgramManager* manager)
    : Program(RK_OpenGL), status_(GLProgramStatus::Invalid), id_(0), allUniformVariablesSet_(false),
      shaders_(shaders), manager_(manager) {}

GLProgram::~GLProgram() { destroyGLProgram(this); }

bool GLProgram::isValid() const { return (status_ == GLProgramStatus::Linked); }

const std::set<std::shared_ptr<Shader>>& GLProgram::getShaders() const { return shaders_; }

void GLProgram::addShader(const std::shared_ptr<Shader>& shader) {
  status_ = GLProgramStatus::Created;
  shaders_.insert(shader);
}

bool GLProgram::removeShader(const std::shared_ptr<Shader>& shader) {
  status_ = GLProgramStatus::Created;
  return shaders_.erase(shader);
}

const std::unordered_map<std::string, GLProgram::GLUniformInfo>&
GLProgram::getUniformVariables() const {
  return uniformInfoMap_;
}

std::unordered_map<std::string, GLProgram::GLUniformInfo>& GLProgram::getUniformVariables() {
  return uniformInfoMap_;
}

GLProgramManager* GLProgram::getManager() const { return manager_; }

unsigned int GLProgram::getID() const { return id_; }

GLProgramStatus GLProgram::getStatus() const { return status_; }

void GLProgram::bind() {
  if(!isValid())
    manager_->makeValid(dyn_pointer_cast<GLProgram>(shared_from_this()));

  glUseProgram(id_);
}

void GLProgram::unbind() { glUseProgram(0); }

bool GLProgram::checkUniformVariables() {
  if(allUniformVariablesSet_)
    return true;

  allUniformVariablesSet_ = true;
  for(const auto& nameInfoPair : uniformInfoMap_) {
    if(!nameInfoPair.second.ValueSet) {
      allUniformVariablesSet_ = false;
      LOG(WARNING) << "Unset uniform variable '" << nameInfoPair.first << "' in program (ID=" << id_
                   << ")";
    }
  }
  return allUniformVariablesSet_;
}

bool GLProgram::isTextureSampler(const std::string& name) const {
  auto it = uniformInfoMap_.find(name);
  return (it != uniformInfoMap_.end() ? it->second.TextureUnit != -1 : false);
}

const std::string& GLProgram::getTextureSampler(int textureUnit) const {
  auto it = textureSamplers_.find(textureUnit);
  return (it != textureSamplers_.end() ? it->second : GLProgram::EmptyString);
}

std::string GLProgram::getLog() const {
  if(status_ == GLProgramStatus::Invalid)
    return "invalid";

  std::stringstream ss;
  ss << "Program Info Log (ID = " << id_ << ")\n";

  int index = 0;
  for(const std::shared_ptr<Shader>& shader : shaders_) {
    ss << core::format("  %-40s : %s\n", "GL_ATTACHED_SHADER_" + std::to_string(index),
                       "ID = " + std::to_string(dyn_cast<GLShader>(shader.get())->getID()) +
                           ", type = " + Shader::shaderTypeToString(shader->getType()));
    ++index;
  }

  int info = 0;
  for(auto param :
      {GL_PROGRAM_SEPARABLE, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_LINK_STATUS, GL_VALIDATE_STATUS,
       GL_DELETE_STATUS, GL_ACTIVE_ATTRIBUTES, GL_ACTIVE_UNIFORMS, GL_ACTIVE_UNIFORM_BLOCKS,
       GL_TRANSFORM_FEEDBACK_BUFFER_MODE, GL_TRANSFORM_FEEDBACK_VARYINGS}) {
    glGetProgramiv(id_, param, &info);
    ss << core::format("  %-40s : %d\n", param, info);
  }
  return ss.str();
}

std::string GLProgram::toString() const {
  return core::format(
      "GLProgram[\n"
      "  status = %s,\n"
      "  id = %s,\n"
      "  uniformInfoMap = %s,\n"
      "  textureSamplers = %s,\n"
      "  shaders = %s\n"
      "]",
      statusToString(status_), id_,
      !uniformInfoMap_.empty()
          ? core::indent(core::toStringRange(uniformInfoMap_,
                                             [](const auto& pair) {
                                               std::stringstream ss;
                                               ss << "name = " << pair.first
                                                  << ", type = " << pair.second.Type;
                                               return ss.str();
                                             }))
          : "null",
      !textureSamplers_.empty()
          ? core::indent(core::toStringRange(textureSamplers_,
                                             [](const auto& pair) {
                                               std::stringstream ss;
                                               ss << "name = " << pair.second
                                                  << ", textureUnit = " << pair.first;
                                               return ss.str();
                                             }))
          : "null",
      !shaders_.empty()
          ? core::indent(core::toStringRange(
                shaders_, [](const auto& shader) { return core::indent(shader->toString()); }))
          : "null");
}

void GLProgram::reportWarningForInvalidUniformVariable(const std::string& name) {
  if(reportedWarningForInvalidUniformVariable_.count(name) == 0) {
    LOG(WARNING) << "Setting non-existing uniform variable '" << name << "' in program (ID=" << id_
                 << ")";
    reportedWarningForInvalidUniformVariable_.emplace(name);
  }
}

namespace {

// Generic "address of" computation
template <class T, bool IsFundamental>
struct ComputeReturnType {
  using type = const T*;
};

template <class T>
struct ComputeReturnType<T, false> {
  using type = const typename T::value_type*;
};

template <class T, bool IsFundamental>
struct AddressOf {
  static typename ComputeReturnType<T, IsFundamental>::type apply(const T& value) {
    return std::addressof(value);
  }
};

template <class T>
struct AddressOf<T, false> {
  static typename ComputeReturnType<T, false>::type apply(const T& value) {
    return math::value_ptr(value);
  }
};

template <class T, bool IsFundamental = std::is_fundamental<T>::value>
inline typename ComputeReturnType<T, IsFundamental>::type addressOf(const T& value) {
  return AddressOf<T, IsFundamental>::apply(value);
}

template <class T>
struct GLTypeCompat {};

/// @brief Create a setter implementation for a uniform variable of `TYPE`
///
/// @param TYPE    C++ type of the uniform variable
/// @param FUNC    OpenGL function invoked to set the uniform variable
/// @param ...     GLenum of types which are compatible with `TYPE` (e.g `int` can be used to set
///                `GL_INT` as well as `GL_SAMPLER_2D`.
///
#define SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(TYPE, FUNC, ...)                                         \
  template <>                                                                                      \
  struct GLTypeCompat<TYPE> {                                                                      \
    static constexpr bool isCompatible(GLenum type) noexcept {                                     \
      return core::tuple_has_value(std::make_tuple(__VA_ARGS__), type);                            \
    }                                                                                              \
    static constexpr const char* getTypeName() { return #TYPE; }                                   \
  };                                                                                               \
                                                                                                   \
  bool setUniformVariableImpl(GLProgram* program, const std::string& name, const TYPE& value) {    \
    SEQUOIA_ASSERT(program->getStatus() == GLProgramStatus::Linked);                               \
    auto it = program->getUniformVariables().find(name);                                           \
    if(it == program->getUniformVariables().end()) {                                               \
      program->reportWarningForInvalidUniformVariable(name);                                       \
      return false;                                                                                \
    }                                                                                              \
    GLProgram::GLUniformInfo& info = it->second;                                                   \
    if(!GLTypeCompat<TYPE>::isCompatible(info.Type))                                               \
      SEQUOIA_THROW(RenderSystemException, "failed to set uniform variable '%s' in program "       \
                                           "(ID=%i), cannot convert type '%s' to '%s'",            \
                    name, program->getID(), GLTypeCompat<TYPE>::getTypeName(), info.Type);         \
    if(info.Size != 1)                                                                             \
      SEQUOIA_THROW(                                                                               \
          RenderSystemException,                                                                   \
          "invalid rank '%i' of uniform variable '%s' in program (ID=%i), expected '%i'", 1, name, \
          program->getID(), info.Size);                                                            \
    FUNC(program->getID(), info.Location, info.Size, addressOf(value));                            \
    info.ValueSet = true;                                                                          \
    return true;                                                                                   \
  }

SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(int, glProgramUniform1iv, GL_INT, GL_SAMPLER_2D)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(float, glProgramUniform1fv, GL_FLOAT)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(bool, ([](GLuint program, GLint location, GLsizei count,
                                            const bool* value) {
                                    GLboolean valuePtr = *value;
                                    glProgramUniform1bv(program, location, count, &valuePtr);
                                  }),
                                  GL_BOOL)

SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::vec2, glProgramUniform2fv, GL_FLOAT_VEC2)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::vec3, glProgramUniform3fv, GL_FLOAT_VEC3)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::vec4, glProgramUniform4fv, GL_FLOAT_VEC4)

SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::mat2, ([](GLuint program, GLint location, GLsizei count,
                                                  const GLfloat* value) {
                                    glProgramUniformMatrix2fv(program, location, count, false,
                                                              value);
                                  }),
                                  GL_FLOAT_MAT2)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::mat3, ([](GLuint program, GLint location, GLsizei count,
                                                  const GLfloat* value) {
                                    glProgramUniformMatrix3fv(program, location, count, false,
                                                              value);
                                  }),
                                  GL_FLOAT_MAT3)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::mat4, ([](GLuint program, GLint location, GLsizei count,
                                                  const GLfloat* value) {
                                    glProgramUniformMatrix4fv(program, location, count, false,
                                                              value);
                                  }),
                                  GL_FLOAT_MAT4)

#undef SEQUOIA_SET_UNIFORM_VARIABLE_IMPL

} // anonymous namespace

bool GLProgram::setUniformVariable(const std::string& name, const UniformVariable& variable) {
  switch(variable.getType()) {
#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  case UniformType::Name:                                                                          \
    return setUniformVariableImpl(this, name, variable.get<Type>());
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  default:
    sequoia_unreachable("invalid type");
  };
  return false;
}

void destroyGLProgram(GLProgram* program) noexcept {
  if(program->status_ == GLProgramStatus::Invalid)
    return;

  LOG(DEBUG) << "Deleting program (ID=" << program->id_ << ")";
  glDeleteProgram(program->id_);
  program->id_ = 0;
  program->status_ = GLProgramStatus::Invalid;
}

} // namespace render

} // namespace sequoia
