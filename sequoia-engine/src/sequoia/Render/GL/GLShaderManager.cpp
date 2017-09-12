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
#include "sequoia/Core/Logging.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLShader.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <sstream>

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

  LOG(DEBUG) << "Loading shader from disk \"" << shader->file_->getPath() << "\"";

  // Get the shader source
  shader->code_ = shader->file_->getDataAsString();
  if(shader->code_.empty())
    SEQUOIA_THROW(RenderSystemException, "empty shader source: '%s'", shader->file_->getPath());

  // Register the shader within OpenGL
  shader->id_ = glCreateShader(GLShader::getGLShaderType(shader->getType()));
  if(shader->id_ == 0)
    SEQUOIA_THROW(RenderSystemException, "cannot create shader: '%s'", shader->file_->getPath());

  LOG(DEBUG) << "Created shader (ID=" << shader->id_ << ") from source \""
             << shader->file_->getPath() << "\"";

  LOG(DEBUG) << "Compiling shader (ID=" << shader->id_ << ") ...";

  // Compile shader
  const char* code = shader->code_.c_str();
  glShaderSource(shader->id_, 1, &code, nullptr);
  glCompileShader(shader->id_);

  // Check compilation
  int compileStatus;
  glGetShaderiv(shader->id_, GL_COMPILE_STATUS, &compileStatus);
  if(!compileStatus) {
    int infoLogLength;
    glGetShaderiv(shader->id_, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::vector<char> errorMessage(infoLogLength + 1);
    glGetShaderInfoLog(shader->id_, infoLogLength, NULL, &errorMessage[0]);

    std::string msg(errorMessage.data(), errorMessage.size());

    // Extract 'Y(X)' where 'X' is the line number and 'Y' is the column number
    int lbrace = msg.find_first_of('('), rbrace = msg.find_first_of(')');
    try {
      int row = boost::lexical_cast<int>(msg.substr(lbrace + 1, rbrace - lbrace - 1));
      std::string line = getRow(shader->code_, row);

      if(!line.empty())
        msg += "\n" + line + "\n";

    } catch(const boost::bad_lexical_cast&) {
    }

    SEQUOIA_THROW(RenderSystemException, "failed to compile shader: %s", msg);
  }

  LOG(DEBUG) << "Successfully compiled shader (ID=" << shader->id_ << ")";
}

std::shared_ptr<GLShader> GLShaderManager::create(GLShader::ShaderType type,
                                                  const std::shared_ptr<File>& file) {
  SEQUOIA_LOCK_GUARD(mutex_);

  auto it = fileLookupMap_.find(file);

  if(it != fileLookupMap_.end())
    return shaderList_[it->second];

  shaderList_.emplace_back(std::make_shared<GLShader>(type, file));
  fileLookupMap_[file] = shaderList_.size() - 1;
  return shaderList_.back();
}

void GLShaderManager::remove(const std::shared_ptr<GLShader>& shader) noexcept {
  SEQUOIA_LOCK_GUARD(mutex_);
  shaderList_.erase(std::remove(shaderList_.begin(), shaderList_.end(), shader), shaderList_.end());
  fileLookupMap_.erase(shader->getFile());
}

} // namespace render

} // namespace sequoia
