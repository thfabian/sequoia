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
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgram.h"
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

GLProgramManager* GLProgram::getManager() const { return manager_; }

unsigned int GLProgram::getID() const { return id_; }

GLProgramStatus GLProgram::getStatus() const { return status_; }

void GLProgram::bind() {
  if(!isValid())
    manager_->makeValid(dyn_pointer_cast<GLProgram>(shared_from_this()));
  
  if(!allUniformVariablesSet_)
    checkUniformVariables();

  glUseProgram(id_);
}

void GLProgram::unbind() { glUseProgram(0); }

bool GLProgram::checkUniformVariables() {
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
      !shaders_.empty()
          ? core::indent(core::toStringRange(
                shaders_, [](const auto& shader) { return core::indent(shader->toString()); }))
          : "null");
}

namespace {

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
  static typename ComputeReturnType<T, IsFundamental>::type doit(const T& value) {
    return std::addressof(value);
  }
};

template <class T>
struct AddressOf<T, false> {
  static typename ComputeReturnType<T, false>::type doit(const T& value) {
    return math::value_ptr(value);
  }
};

template <class T, bool IsFundamental = std::is_fundamental<T>::value>
inline typename ComputeReturnType<T, IsFundamental>::type addressOf(const T& value) {
  return AddressOf<T, IsFundamental>::doit(value);
}

} // anonymous namespace

#define SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(TYPE, GL_TYPE, FUNC)                                     \
  bool GLProgram::setUniformVariable(const std::string& name, const TYPE& value) {                 \
    SEQUOIA_ASSERT(status_ == GLProgramStatus::Linked);                                            \
    auto it = uniformInfoMap_.find(name);                                                          \
    if(it == uniformInfoMap_.end()) {                                                              \
      LOG(WARNING) << "Setting non-existing uniform variable '" << name << "'";                    \
      return false;                                                                                \
    }                                                                                              \
    GLUniformInfo& info = it->second;                                                              \
    if(info.Type != GL_TYPE)                                                                       \
      SEQUOIA_THROW(                                                                               \
          RenderSystemException,                                                                   \
          "invalid type '%s' of uniform variable '%s' in program (ID=%i), expected '%s'", GL_TYPE, \
          name, id_, info.Type);                                                                   \
    if(info.Size != 1)                                                                             \
      SEQUOIA_THROW(                                                                               \
          RenderSystemException,                                                                   \
          "invalid rank '%i' of uniform variable '%s' in program (ID=%i), expected '%i'", 1, name, \
          id_, info.Size);                                                                         \
    if(info.ValueSet)                                                                              \
      LOG(WARNING) << "Overriding uniform variable '" << name << "'";                              \
    FUNC(id_, info.Location, info.Size, addressOf(value));                                         \
    info.ValueSet = true;                                                                          \
    return true;                                                                                   \
  }

SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(float, GL_FLOAT, glProgramUniform1fv)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(int, GL_INT, glProgramUniform1iv)

SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::fvec2, GL_FLOAT_VEC2, glProgramUniform2fv)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::fvec3, GL_FLOAT_VEC3, glProgramUniform3fv)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::fvec4, GL_FLOAT_VEC4, glProgramUniform4fv)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::ivec2, GL_INT_VEC2, glProgramUniform2iv)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::ivec3, GL_INT_VEC3, glProgramUniform3iv)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::ivec4, GL_INT_VEC4, glProgramUniform4iv)

SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::fmat2, GL_FLOAT_MAT2,
                                  ([](GLuint program, GLint location, GLsizei count,
                                      const GLfloat* value) {
                                    glProgramUniformMatrix2fv(program, location, count, false,
                                                              value);
                                  }))
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::fmat3, GL_FLOAT_MAT3,
                                  ([](GLuint program, GLint location, GLsizei count,
                                      const GLfloat* value) {
                                    glProgramUniformMatrix3fv(program, location, count, false,
                                                              value);
                                  }))
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::fmat4, GL_FLOAT_MAT4,
                                  ([](GLuint program, GLint location, GLsizei count,
                                      const GLfloat* value) {
                                    glProgramUniformMatrix4fv(program, location, count, false,
                                                              value);
                                  }))

#undef SEQUOIA_SET_UNIFORM_VARIABLE_IMPL

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
