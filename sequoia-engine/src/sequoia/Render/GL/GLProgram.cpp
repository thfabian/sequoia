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

#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/STLExtras.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLProgram.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLShader.h"
#include <sstream>

namespace sequoia {

namespace render {

const std::string GLProgram::EmptyString;

GLProgram::GLProgram(const std::set<std::shared_ptr<Shader>>& shaders)
    : Program(RK_OpenGL), id_(0), allUniformVariablesSet_(false), shaders_(shaders) {}

GLProgram::~GLProgram() { destroyGLProgram(this); }

const std::set<std::shared_ptr<Shader>>& GLProgram::getShaders() const { return shaders_; }

const std::unordered_map<std::string, GLProgram::GLUniformInfo>&
GLProgram::getUniformVariables() const {
  return uniformInfoMap_;
}

std::unordered_map<std::string, GLProgram::GLUniformInfo>& GLProgram::getUniformVariables() {
  return uniformInfoMap_;
}

unsigned int GLProgram::getID() const { return id_; }

void GLProgram::bind() {
  SEQUOIA_ASSERT_MSG(isValid(), "binding invalid program");
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
                   << ") with shaders "
                   << core::RangeToString(", ", "{", "}")(shaders_, [](const auto& shader) {
                        return shader->getFile()->getPath();
                      });
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

std::string GLProgram::toString() const {
  return core::format(
      "GLProgram[\n"
      "  valid = {},\n"
      "  id = {},\n"
      "  uniformInfoMap = {},\n"
      "  textureSamplers = {},\n"
      "  shaders = {}\n"
      "]",
      isValid() ? "true" : "false", id_,
      !uniformInfoMap_.empty()
          ? core::indent(core::toStringRange(uniformInfoMap_,
                                             [](const auto& pair) {
                                               std::stringstream s;
                                               s << "name = " << pair.first
                                                 << ", type = " << pair.second.Type;
                                               return s.str();
                                             }))
          : "null",
      !textureSamplers_.empty()
          ? core::indent(core::toStringRange(textureSamplers_,
                                             [](const auto& pair) {
                                               std::stringstream s;
                                               s << "name = " << pair.second
                                                 << ", textureUnit = " << pair.first;
                                               return s.str();
                                             }))
          : "null",
      !shaders_.empty()
          ? core::indent(core::toStringRange(
                shaders_, [](const auto& shader) { return core::indent(shader->toString()); }))
          : "null");
}

void GLProgram::makeValidImpl() { getGLRenderer().getProgramManager()->makeValid(this); }

namespace {

// Generic value type computation
template <class T, bool IsFundamental>
struct ComputeValueTypeImpl {
  using type = T;
};

template <class T>
struct ComputeValueTypeImpl<T, false> {
  using type = typename T::value_type;
};

template <class T>
struct ComputeValueType {
  using type = typename ComputeValueTypeImpl<T, std::is_fundamental<T>::value>::type;
};

// Generic "address of" computation
template <class T>
struct ComputeReturnType {
  using type = const typename ComputeValueType<T>::type*;
};

template <class T, bool IsFundamental>
struct AddressOf {
  static typename ComputeReturnType<T>::type apply(const T& value) { return std::addressof(value); }
};

template <class T>
struct AddressOf<T, false> {
  static typename ComputeReturnType<T>::type apply(const T& value) {
    return math::value_ptr(value);
  }
};

template <class T, bool IsFundamental = std::is_fundamental<T>::value>
inline typename ComputeReturnType<T>::type addressOf(const T& value) {
  return AddressOf<T, IsFundamental>::apply(value);
}

// Copy data if we have boolean vector
template <class T>
const T* getDataFromVector(const std::vector<T>& vec) {
  return vec.data();
}

template <>
const bool* getDataFromVector(const std::vector<bool>& vec) {
  bool* data = new bool[vec.size()];
  for(int i = 0; i < vec.size(); ++i)
    data[i] = vec[i];
  return data;
}

/// @brief Wrapper for `std::vector<T>` which disables `std::vector<bool>` optimizations
template <class T, bool IsBoolVector = std::is_same<T, bool>::value>
class VectorWrapper : public NonCopyable {
public:
  using ValueType = typename ComputeValueType<T>::type;

  VectorWrapper(const std::vector<T>& vec) : size_(vec.size()) {
    // This will copy the data if we have a boolean vector and extract the data pointer to the first
    // elements data
    data_ = addressOf(getDataFromVector(vec)[0]);
  }

  ~VectorWrapper() {
    if(IsBoolVector)
      delete[] data_;
  }

  /// @brief Return a pointer to the data
  ///
  /// Note that if we have a vector of e.g math::vec3 (i.e a composed type), this returns a pointer
  /// to the first element of math::vec3 (math::vec3.x) of the first element in the full vector.
  const ValueType* data() const noexcept { return reinterpret_cast<const ValueType*>(data_); }

  /// @brief Get the number of elements in the vector
  std::size_t size() const noexcept { return size_; }

private:
  const ValueType* data_;
  std::size_t size_;
};

template <class T>
struct GLTypeCompat {};

template <class T>
struct UniformVariableSetter {};

/// @brief Create a setter implementation for a uniform variable of `TYPE` as well as the vector
/// version of it (i.e `std::vector<TYPE>`)
///
/// The function takes a pointer to the data and its array size (which is 1 or scalars), checks that
/// the type and size of the uniform variable recorded in the program matches the ones provided and
/// uploads the data to the GPU.
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
  template <>                                                                                      \
  struct UniformVariableSetter<TYPE> {                                                             \
    template <class DataType = typename ComputeValueType<TYPE>::type>                              \
    static bool apply(GLProgram* program, const std::string& name, const DataType* data,           \
                      std::size_t rank) {                                                          \
      auto it = program->getUniformVariables().find(name);                                         \
      if(it == program->getUniformVariables().end()) {                                             \
        return false;                                                                              \
      }                                                                                            \
      GLProgram::GLUniformInfo& info = it->second;                                                 \
      if(!GLTypeCompat<TYPE>::isCompatible(info.Type))                                             \
        SEQUOIA_THROW(RenderSystemException, "failed to set uniform variable '{}' in program "     \
                                             "(ID={}), cannot convert type '{}' to '{}'",          \
                      name, program->getID(), GLTypeCompat<TYPE>::getTypeName(), info.Type);       \
      if(info.Rank != rank)                                                                        \
        SEQUOIA_THROW(RenderSystemException, "invalid rank (size of array) '{}' of uniform "       \
                                             "variable '{}' in program (ID={}), expected '{}'",    \
                      1, name, program->getID(), info.Rank);                                       \
      FUNC(program->getID(), info.Location, info.Rank, data);                                      \
      info.ValueSet = true;                                                                        \
      return true;                                                                                 \
    }                                                                                              \
  };

// Scalars
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(int, glProgramUniform1iv, GL_INT, GL_SAMPLER_2D)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(float, glProgramUniform1fv, GL_FLOAT)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(bool, ([](GLuint p, GLint l, GLsizei c, const bool* v) {
                                    GLboolean valuePtr = *v;
                                    glProgramUniform1bv(p, l, c, &valuePtr);
                                  }),
                                  GL_BOOL)

// Vectors
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::vec2, glProgramUniform2fv, GL_FLOAT_VEC2)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::vec3, glProgramUniform3fv, GL_FLOAT_VEC3)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::vec4, glProgramUniform4fv, GL_FLOAT_VEC4)

// Matrices
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::mat2, ([](GLuint p, GLint l, GLsizei c, const GLfloat* v) {
                                    glProgramUniformMatrix2fv(p, l, c, false, v);
                                  }),
                                  GL_FLOAT_MAT2)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::mat3, ([](GLuint p, GLint l, GLsizei c, const GLfloat* v) {
                                    glProgramUniformMatrix3fv(p, l, c, false, v);
                                  }),
                                  GL_FLOAT_MAT3)
SEQUOIA_SET_UNIFORM_VARIABLE_IMPL(math::mat4, ([](GLuint p, GLint l, GLsizei c, const GLfloat* v) {
                                    glProgramUniformMatrix4fv(p, l, c, false, v);
                                  }),
                                  GL_FLOAT_MAT4)

#undef SEQUOIA_SET_UNIFORM_VARIABLE_IMPL

} // anonymous namespace

bool GLProgram::setUniformVariable(const std::string& name, const UniformVariable& variable) {
  SEQUOIA_ASSERT_MSG(isValid(), "setting uniform variable of invalid program");

  switch(variable.getType()) {
#define UNIFORM_VARIABLE_TYPE(Type, Name)                                                          \
  case UniformType::Name:                                                                          \
    return UniformVariableSetter<Type>::apply(this, name, addressOf(variable.get<Type>()), 1);     \
  case UniformType::VectorOf##Name: {                                                              \
    VectorWrapper<Type> vec(variable.get<std::vector<Type>>());                                    \
    return UniformVariableSetter<Type>::apply(this, name + "[0]", vec.data(), vec.size());         \
  }
#include "sequoia/Render/UniformVariable.inc"
#undef UNIFORM_VARIABLE_TYPE
  default:
    sequoia_unreachable("invalid type");
  };
  return false;
}

void destroyGLProgram(GLProgram* program) noexcept {
  if(!program->isValid())
    return;

  LOG(DEBUG) << "Deleting program (ID=" << program->id_ << ")";
  glDeleteProgram(program->id_);
  program->id_ = 0;
}

} // namespace render

} // namespace sequoia
