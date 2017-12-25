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

#include "sequoia-engine/Core/Logging.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Render/GL/GLShader.h"
#include "sequoia-engine/Render/GL/GLShaderManager.h"
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <sstream>

#include <iostream>

namespace sequoia {

namespace render {

/// @brief Extract a single row from `source`
static std::string getRow(const std::string& source, int rowIdx) {
  int row = 1;
  std::size_t start = 0, end = std::string::npos;
  for(std::size_t i = 0; i < source.size(); ++i) {
    if(source[i] == '\n') {
      end = i;
      if(row == rowIdx)
        return source.substr(start, end - start);
      start = end + 1;
      ++row;
    }
  }
  return std::string();
}

GLShaderManager::~GLShaderManager() {}

void GLShaderManager::makeValid(GLShader* shader) {
  SEQUOIA_ASSERT_MSG(!shader->isValid(), "shader already initialized");

  Log::debug("Loading shader from disk \"{}\"", shader->filename_);

  // Get the shader source
  if(shader->source_.empty())
    SEQUOIA_THROW(RenderSystemException, "empty shader source: '{}'", shader->filename_);

  // Register the shader within OpenGL
  shader->id_ = glCreateShader(GLShader::getGLShaderType(shader->getType()));
  if(shader->id_ == 0)
    SEQUOIA_THROW(RenderSystemException, "cannot create shader: '{}'", shader->filename_);

  Log::debug("Created shader (ID={}) from source \"{}\"", shader->id_, shader->filename_);

  Log::debug("Compiling shader (ID={}) ...", shader->id_);

  // Compile shader
  const char* code = shader->source_.c_str();
  GLint length = shader->source_.size();
  glShaderSource(shader->id_, 1, &code, &length);
  glCompileShader(shader->id_);

  // Check compilation
  GLint status = 0;
  glGetShaderiv(shader->id_, GL_COMPILE_STATUS, &status);
  if(!status) {
    GLint infoLogLength = 0;
    glGetShaderiv(shader->id_, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::vector<char> infoLog(infoLogLength + 1);
    glGetShaderInfoLog(shader->id_, infoLogLength, NULL, &infoLog[0]);

    std::string reason(infoLog.data(), infoLog.size());

    // Extract 'Y(X)' where 'X' is the line number and 'Y' is the column number
    int lbrace = reason.find_first_of('('), rbrace = reason.find_first_of(')');
    try {
      int row = boost::lexical_cast<int>(reason.substr(lbrace + 1, rbrace - lbrace - 1));
      std::string line = getRow(shader->source_, row);

      if(!line.empty())
        reason += "\n  " + line + "\n";

    } catch(const boost::bad_lexical_cast&) {
    }

    SEQUOIA_THROW(RenderSystemException, "failed to compile shader (ID={}):\n{}", shader->id_, reason);
  }

  Log::debug("Successfully compiled shader (ID={})", shader->id_);
}

std::shared_ptr<GLShader> GLShaderManager::create(GLShader::ShaderType type,
                                                  const std::string& filename,
                                                  const std::string& source) {
  SEQUOIA_LOCK_GUARD(mutex_);

  auto it = filenameLookupMap_.find(filename);

  if(it != filenameLookupMap_.end())
    return shaderList_[it->second];

  shaderList_.emplace_back(std::make_shared<GLShader>(type, filename, source));
  filenameLookupMap_[filename] = shaderList_.size() - 1;
  return shaderList_.back();
}

void GLShaderManager::remove(const std::shared_ptr<GLShader>& shader) noexcept {
  SEQUOIA_LOCK_GUARD(mutex_);
  shaderList_.erase(std::remove(shaderList_.begin(), shaderList_.end(), shader), shaderList_.end());
  filenameLookupMap_.erase(shader->getFilename());
}

} // namespace render

} // namespace sequoia
