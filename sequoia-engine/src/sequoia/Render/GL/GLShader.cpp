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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLShader.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include <sstream>

#include "sequoia/Render/GL/GLRenderer.h"

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

GLShader::GLShader(Shader::ShaderType type, const std::shared_ptr<File>& file)
    : Shader(RK_OpenGL, type), id_(0), code_(), file_(file) {}

GLShader::~GLShader() { destroyGLShader(this); }

unsigned int GLShader::getID() const { return id_; }

const std::shared_ptr<File>& GLShader::getFile() const { return file_; }

std::string GLShader::getSourceCode() const { return code_; }

std::string GLShader::toString() const {
  return core::format("GLShader[\n"
                      "  valid = %s,\n"
                      "  type = %s,\n"
                      "  id = %s,\n"
                      "  path = \"%s\"\n"
                      "]",
                      isValid() ? "true" : "false", shaderTypeToString(getType()), id_,
                      UtfString(file_->getPath()).toAnsiString());
}

void GLShader::makeValidImpl() {
  getGLRenderSystem().getRenderer()->getShaderManager()->makeValid(this);
}

void destroyGLShader(GLShader* shader) noexcept {
  if(!shader->isValid())
    return;

  LOG(DEBUG) << "Deleting shader (ID=" << shader->id_ << ")";

  SEQUOIA_ASSERT(shader->id_ != 0);
  glDeleteShader(shader->id_);
  shader->id_ = 0;
}

} // namespace render

} // namespace sequoia
