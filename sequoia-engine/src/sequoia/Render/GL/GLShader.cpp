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

#include "sequoia/Render/GL/GLShader.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GLShaderManager.h"

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

GLShader::GLShader(Shader::ShaderType type, const platform::String& path, GLShaderManager* manager)
    : Shader(RK_OpenGL, type), status_(GLShaderStatus::OnDisk), id_(0), manager_(manager), code_(),
      path_(path) {}

bool GLShader::isValid() const { return status_ == GLShaderStatus::Compiled; }

unsigned int GLShader::getID() const { return id_; }

platform::String GLShader::getSourcePath() const { return path_; }

std::string GLShader::getSourceCode() const { return code_; }

std::string GLShader::toString() const { return manager_->getInfoLog(this); }

GLShaderStatus GLShader::getStatus() const { return status_; }

GLShaderManager* GLShader::getManager() const { return manager_; }

} // namespace render

} // namespace sequoia
