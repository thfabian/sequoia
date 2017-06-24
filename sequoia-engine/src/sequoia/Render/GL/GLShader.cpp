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
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GLShader.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include <sstream>

namespace sequoia {

namespace render {

GLenum GLShader::getGLShaderType(Shader::ShaderType type) {
  switch(type) {
  case ST_Compute:
    return GL_COMPUTE_SHADER;
  case ST_Vertex:
    return GL_VERTEX_SHADER;
  case ST_TessControl:
    return GL_TESS_CONTROL_SHADER;
  case ST_TessEvaluation:
    return GL_TESS_EVALUATION_SHADER;
  case ST_Geometry:
    return GL_GEOMETRY_SHADER;
  case ST_Fragment:
    return GL_FRAGMENT_SHADER;
  default:
    sequoia_unreachable("invalid ShaderType");
  }
}

static const char* statusToString(GLShaderStatus status) {
  switch(status) {
  case GLShaderStatus::OnDisk:
    return "OnDisk";
  case GLShaderStatus::InMemory:
    return "InMemory";
  case GLShaderStatus::Created:
    return "Created";
  case GLShaderStatus::Compiled:
    return "Compiled";
  default:
    sequoia_unreachable("invalid GLShaderStatus");
  }
}

GLShader::GLShader(Shader::ShaderType type, const std::shared_ptr<File>& file,
                   GLShaderManager* manager)
    : Shader(RK_OpenGL, type), status_(GLShaderStatus::OnDisk), id_(0), manager_(manager), code_(),
      file_(file) {}

GLShader::~GLShader() { destroyGLShader(this); }

bool GLShader::isValid() const { return status_ == GLShaderStatus::Compiled; }

unsigned int GLShader::getID() const { return id_; }

const std::shared_ptr<File>& GLShader::getFile() const { return file_; }

std::string GLShader::getSourceCode() const { return code_; }

std::string GLShader::getLog() const {
  if(status_ < GLShaderStatus::Created)
    return "invalid";

  std::stringstream ss;
  ss << "Shader Info Log (ID = " << id_ << ")\n";

  int info = 0;
  for(auto param : {GL_COMPILE_STATUS, GL_DELETE_STATUS}) {
    glGetShaderiv(id_, param, &info);
    ss << core::format("  %-40s : %d\n", param, info);
  }
  return ss.str();
}

std::string GLShader::toString() const {
  return core::format("GLShader[\n"
                      "  type = %s,\n"
                      "  status = %s,\n"
                      "  id = %s,\n"
                      "  path = \"%s\"\n"
                      "]",
                      shaderTypeToString(type_), statusToString(status_), id_,
                      UtfString(file_->getPath()).toAnsiString());
}

GLShaderStatus GLShader::getStatus() const { return status_; }

GLShaderManager* GLShader::getManager() const { return manager_; }

void destroyGLShader(GLShader* shader) noexcept {
  if(shader->status_ <= GLShaderStatus::InMemory)
    return;

  LOG(DEBUG) << "Deleting shader (ID=" << shader->id_ << ")";

  SEQUOIA_ASSERT(shader->id_ != 0);
  glDeleteShader(shader->id_);
  shader->id_ = 0;

  shader->status_ = GLShaderStatus::InMemory;
}

} // namespace render

} // namespace sequoia
