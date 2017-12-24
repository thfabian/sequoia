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

#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Render/GL/GLRenderSystem.h"
#include "sequoia-engine/Render/GL/GLRenderer.h"
#include "sequoia-engine/Render/GL/GLShader.h"
#include "sequoia-engine/Render/GL/GLShaderManager.h"
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

GLShader::GLShader(ShaderType type, const std::string& filename, const std::string& source)
    : Shader(RK_OpenGL, type), id_(0), source_(source), filename_(filename) {}

GLShader::~GLShader() { destroyGLShader(this); }

unsigned int GLShader::getID() const { return id_; }

const std::string& GLShader::getFilename() const { return filename_; }

const std::string& GLShader::getSourceCode() const { return source_; }

std::string GLShader::toString() const {
  return core::format("GLShader[\n"
                      "  valid = {},\n"
                      "  type = {},\n"
                      "  id = {},\n"
                      "  filename = \"{}\"\n"
                      "]",
                      isValid() ? "true" : "false", shaderTypeToString(getType()), id_, filename_);
}

void GLShader::makeValidImpl() { getGLRenderer().getShaderManager()->makeValid(this); }

void destroyGLShader(GLShader* shader) noexcept {
  if(!shader->isValid())
    return;

  Log::debug("Deleting shader (ID={})", shader->id_);

  SEQUOIA_ASSERT(shader->id_ != 0);
  glDeleteShader(shader->id_);
  shader->id_ = 0;
}

} // namespace render

} // namespace sequoia
