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
#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLProgramManager.h"
#include "sequoia/Render/GL/GLShader.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include <algorithm>
#include <boost/functional/hash.hpp>

namespace sequoia {

namespace render {

/// @brief Convert `type` to string
static const char* typeToString(GLenum type);

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

    glLinkProgram(program->id_);
    if(glGetError() != GL_NO_ERROR)
      SEQUOIA_THROW(RenderSystemException, "failed to link program");
    program->status_ = GLProgramStatus::Linked;

    for(Shader* shader : program->getShaders()) {
      GLShader* glshader = dyn_cast<GLShader>(shader);
      glDetachShader(program->id_, glshader->getID());
    }

    // Set uniform variables
    setUniforms(program);

    LOG(DEBUG) << "Successfully linked program (ID=" << program->id_ << ")";
  }
}

void GLProgramManager::setVertexAttribName(GLProgram* program, GLVertexAttrib attrib,
                                           const std::string& name) {
  make(program, GLProgramStatus::Created);
  glBindAttribLocation(program->getID(), (GLuint)attrib, name.c_str());

  // Attribute bindings need relinking
  program->status_ = GLProgramStatus::Created;
}

void GLProgramManager::destroy(GLProgram* program) {
  if(program->status_ == GLProgramStatus::Invalid)
    return;

  LOG(DEBUG) << "Deleting program (ID=" << program->id_ << ")";
  glDeleteProgram(program->id_);
  program->id_ = 0;
  program->status_ = GLProgramStatus::Invalid;
}

void GLProgramManager::setUniforms(GLProgram* program) const {
  LOG(DEBUG) << "Setting uniform variables of program (ID=" << program->id_ << ")";
  SEQUOIA_ASSERT_MSG(program->status_ == GLProgramStatus::Linked, "");

  program->uniformMap_.clear();

  int activeUniforms = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_UNIFORMS, &activeUniforms);
  LOG(DEBUG) << "Program has " << activeUniforms << " active uniform variables";

  int activeUniformMaxLength = 0;
  glGetProgramiv(program->id_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &activeUniformMaxLength);
  auto name = std::make_unique<char[]>(activeUniformMaxLength);

  for(int index = 0; index < activeUniforms; ++index) {
    GLenum type;
    GLint size, length;
    glGetActiveUniform(program->id_, index, activeUniformMaxLength, &length, &size, &type,
                       name.get());
    GLint location = glGetUniformLocation(program->id_, name.get());

    LOG(DEBUG) << "Uniform variable: name=" << name.get() << ", type=" << typeToString(type)
               << ", location=" << location;

    if(location != -1)
      program->uniformMap_.emplace(name.get(), GLProgram::GLUniform{type, size, location});
  }

  LOG(DEBUG) << "Successfully set uniform variables of program (ID=" << program->id_ << ")";
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

std::string GLProgramManager::getInfoLog(const GLProgram* program) const {
  if(program->status_ == GLProgramStatus::Invalid)
    return "invalid";

  std::stringstream ss;
  ss << std::boolalpha;
  ss << "Program Info Log (ID = " << program->id_ << ")\n";

  int i = 1;
  for(Shader* shader : program->shaders_) {
    ss << core::format("  %-40s : %s\n", "GL_ATTACHED_SHADER_" + std::to_string(i),
                       "ID = " + std::to_string(dyn_cast<GLShader>(shader)->getID()) + ", Type = " +
                           Shader::shaderTypeToString(shader->getType()));
    ++i;
  }

  int info = 0;
  for(auto param :
      {GL_PROGRAM_SEPARABLE, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_LINK_STATUS, GL_VALIDATE_STATUS,
       GL_DELETE_STATUS, GL_ACTIVE_ATTRIBUTES, GL_ACTIVE_UNIFORMS, GL_ACTIVE_UNIFORM_BLOCKS,
       GL_TRANSFORM_FEEDBACK_BUFFER_MODE, GL_TRANSFORM_FEEDBACK_VARYINGS}) {
    glGetProgramiv(program->id_, param, &info);
    ss << core::format("  %-40s : %d\n", param, info);
  }
  return ss.str();
}

static const char* typeToString(GLenum type) {
  switch(type) {
  case GL_FLOAT:
    return "float";
  case GL_FLOAT_VEC2:
    return "float_vec2";
  case GL_FLOAT_VEC3:
    return "float_vec3";
  case GL_FLOAT_VEC4:
    return "float_vec4";
  case GL_DOUBLE:
    return "double";
  case GL_DOUBLE_VEC2:
    return "double_vec2";
  case GL_DOUBLE_VEC3:
    return "double_vec3";
  case GL_DOUBLE_VEC4:
    return "double_vec4";
  case GL_IMAGE_1D:
    return "image_1d ";
  case GL_IMAGE_2D_RECT:
    return "image_2d_rect";
  case GL_IMAGE_BUFFER:
    return "image_buffer";
  case GL_IMAGE_1D_ARRAY:
    return "image_1d_array";
  case GL_IMAGE_CUBE_MAP_ARRAY:
    return "image_cube_map_array";
  case GL_IMAGE_2D_MULTISAMPLE:
    return "image_2d_multisample";
  case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
    return "image_2d_multisample_array";
  case GL_INT_IMAGE_1D:
    return "int_image_1d";
  case GL_INT_IMAGE_2D_RECT:
    return "int_image_2d_rect";
  case GL_INT_IMAGE_BUFFER:
    return "int_image_buffer";
  case GL_INT_IMAGE_1D_ARRAY:
    return "int_image_1d_array";
  case GL_INT_IMAGE_CUBE_MAP_ARRAY:
    return "int_image_cube_map_array";
  case GL_INT_IMAGE_2D_MULTISAMPLE:
    return "int_image_2d_multisample";
  case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
    return "int_image_2d_multisample_array";
  case GL_UNSIGNED_INT_IMAGE_1D:
    return "unsigned_int_image_1d";
  case GL_UNSIGNED_INT_IMAGE_2D_RECT:
    return "unsigned_int_image_2d_rect";
  case GL_UNSIGNED_INT_IMAGE_BUFFER:
    return "unsigned_int_image_buffer";
  case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
    return "unsigned_int_image_1d_array";
  case GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY:
    return "unsigned_int_image_cube_map_array";
  case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
    return "unsigned_int_image_2d_multisample";
  case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
    return "unsigned_int_image_2d_multisample_array";
  case GL_IMAGE_2D:
    return "image_2d";
  case GL_IMAGE_3D:
    return "image_3d";
  case GL_IMAGE_CUBE:
    return "image_cube";
  case GL_IMAGE_2D_ARRAY:
    return "image_2d_array";
  case GL_INT_IMAGE_2D:
    return "int_image_2d";
  case GL_INT_IMAGE_3D:
    return "int_image_3d";
  case GL_INT_IMAGE_CUBE:
    return "int_image_cube";
  case GL_INT_IMAGE_2D_ARRAY:
    return "int_image_2d_array";
  case GL_UNSIGNED_INT_IMAGE_2D:
    return "unsigned_int_image_2d";
  case GL_UNSIGNED_INT_IMAGE_3D:
    return "unsigned_int_image_3d";
  case GL_UNSIGNED_INT_IMAGE_CUBE:
    return "unsigned_int_image_cube";
  case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
    return "unsigned_int_image_2d_array";
  case GL_SAMPLER_1D:
    return "sampler_1d";
  case GL_SAMPLER_1D_SHADOW:
    return "sampler_1d_shadow";
  case GL_SAMPLER_1D_ARRAY:
    return "sampler_1d_array";
  case GL_SAMPLER_1D_ARRAY_SHADOW:
    return "sampler_1d_array_shadow";
  case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
    return "sampler_2d_multisample_array";
  case GL_SAMPLER_BUFFER:
    return "sampler_buffer";
  case GL_SAMPLER_2D_RECT:
    return "sampler_2d_rect";
  case GL_SAMPLER_2D_RECT_SHADOW:
    return "sampler_2d_rect_shadow";
  case GL_INT_SAMPLER_1D:
    return "int_sampler_1d";
  case GL_INT_SAMPLER_1D_ARRAY:
    return "int_sampler_1d_array";
  case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
    return "int_sampler_2d_multisample_array";
  case GL_INT_SAMPLER_BUFFER:
    return "int_sampler_buffer";
  case GL_INT_SAMPLER_2D_RECT:
    return "int_sampler_2d_rect";
  case GL_UNSIGNED_INT_SAMPLER_1D:
    return "unsigned_int_sampler_1d";
  case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
    return "unsigned_int_sampler_1d_array";
  case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
    return "unsigned_int_sampler_2d_multisample_array";
  case GL_UNSIGNED_INT_SAMPLER_BUFFER:
    return "unsigned_int_sampler_buffer";
  case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
    return "unsigned_int_sampler_2d_rect";
  case GL_SAMPLER_2D:
    return "sampler_2d";
  case GL_SAMPLER_3D:
    return "sampler_3d";
  case GL_SAMPLER_CUBE:
    return "sampler_cube";
  case GL_SAMPLER_2D_SHADOW:
    return "sampler_2d_shadow";
  case GL_SAMPLER_2D_ARRAY:
    return "sampler_2d_array";
  case GL_SAMPLER_2D_ARRAY_SHADOW:
    return "sampler_2d_array_shadow";
  case GL_SAMPLER_2D_MULTISAMPLE:
    return "sampler_2d_multisample";
  case GL_SAMPLER_CUBE_SHADOW:
    return "sampler_cube_shadow";
  case GL_INT_SAMPLER_2D:
    return "int_sampler_2d";
  case GL_INT_SAMPLER_3D:
    return "int_sampler_3d";
  case GL_INT_SAMPLER_CUBE:
    return "int_sampler_cube";
  case GL_INT_SAMPLER_2D_ARRAY:
    return "int_sampler_2d_array";
  case GL_INT_SAMPLER_2D_MULTISAMPLE:
    return "int_sampler_2d_multisample";
  case GL_UNSIGNED_INT_SAMPLER_2D:
    return "unsigned_int_sampler_2d";
  case GL_UNSIGNED_INT_SAMPLER_3D:
    return "unsigned_int_sampler_3d";
  case GL_UNSIGNED_INT_SAMPLER_CUBE:
    return "unsigned_int_sampler_cube";
  case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
    return "unsigned_int_sampler_2d_array";
  case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
    return "unsigned_int_sampler_2d_multisample";
  case GL_BOOL:
    return "bool";
  case GL_INT:
    return "int";
  case GL_BOOL_VEC2:
    return "bool_vec2";
  case GL_INT_VEC2:
    return "int_vec2";
  case GL_BOOL_VEC3:
    return "bool_vec3";
  case GL_INT_VEC3:
    return "int_vec3";
  case GL_BOOL_VEC4:
    return "bool_vec4";
  case GL_INT_VEC4:
    return "int_vec4";
  case GL_UNSIGNED_INT:
    return "unsigned_int";
  case GL_UNSIGNED_INT_VEC2:
    return "unsigned_int_vec2";
  case GL_UNSIGNED_INT_VEC3:
    return "unsigned_int_vec3";
  case GL_UNSIGNED_INT_VEC4:
    return "unsigned_int_vec4";
  case GL_FLOAT_MAT2:
    return "float_mat2";
  case GL_FLOAT_MAT3:
    return "float_mat3";
  case GL_FLOAT_MAT4:
    return "float_mat4";
  case GL_FLOAT_MAT2x3:
    return "float_mat2x3";
  case GL_FLOAT_MAT2x4:
    return "float_mat2x4";
  case GL_FLOAT_MAT3x2:
    return "float_mat3x2";
  case GL_FLOAT_MAT3x4:
    return "float_mat3x4";
  case GL_FLOAT_MAT4x2:
    return "float_mat4x2";
  case GL_FLOAT_MAT4x3:
    return "float_mat4x3";
  case GL_DOUBLE_MAT2:
    return "double_mat2";
  case GL_DOUBLE_MAT3:
    return "double_mat3";
  case GL_DOUBLE_MAT4:
    return "double_mat4";
  case GL_DOUBLE_MAT2x3:
    return "double_mat2x3";
  case GL_DOUBLE_MAT2x4:
    return "double_mat2x4";
  case GL_DOUBLE_MAT3x2:
    return "double_mat3x2";
  case GL_DOUBLE_MAT3x4:
    return "double_mat3x4";
  case GL_DOUBLE_MAT4x2:
    return "double_mat4x2";
  case GL_DOUBLE_MAT4x3:
    return "double_mat4x3";
  default:
    sequoia_unreachable("invalid type");
  }
}

} // namespace render

} // namespace sequoia
