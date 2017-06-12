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

#include "sequoia/Render/GL/GLShaderManager.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Render/Exception.h"
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

void GLShaderManager::make(const std::shared_ptr<GLShader>& shader,
                           GLShaderStatus requestedStatus) {
  if(shader->status_ == GLShaderStatus::Compiled)
    return;

  if(requestedStatus == GLShaderStatus::OnDisk) {
    destroyGLShader(shader.get());
    return;
  }

  if(shader->status_ == GLShaderStatus::OnDisk) {
    LOG(DEBUG) << "Loading shader from disk \"" << shader->file_->getPath() << "\"";

    shader->code_ = shader->file_->getDataAsString();
    shader->status_ = GLShaderStatus::InMemory;

    if(shader->code_.empty())
      SEQUOIA_THROW(RenderSystemException, "empty shader source: '%s'", shader->file_->getPath());
  }

  if(requestedStatus == GLShaderStatus::InMemory)
    return;

  if(shader->status_ == GLShaderStatus::InMemory) {
    // 0 means it has not yet been registered
    if(shader->id_ != 0)
      idLookupMap_.erase(shader->id_);

    shader->id_ = glCreateShader(GLShader::getGLShaderType(shader->type_));
    if(shader->id_ == 0)
      SEQUOIA_THROW(RenderSystemException, "cannot create shader: '%s'", shader->file_->getPath());

    idLookupMap_.emplace(shader->id_, fileLookupMap_[shader->file_]);

    LOG(DEBUG) << "Created shader (ID=" << shader->id_ << ")";
    shader->status_ = GLShaderStatus::Created;
  }

  if(requestedStatus == GLShaderStatus::Created)
    return;

  if(shader->status_ == GLShaderStatus::Created) {
    LOG(DEBUG) << "Compiling shader (ID=" << shader->id_ << ") ...";

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
    shader->status_ = GLShaderStatus::Compiled;
  }
}

const std::shared_ptr<GLShader>& GLShaderManager::get(unsigned int id) const {
  auto it = idLookupMap_.find(id);
  SEQUOIA_ASSERT_MSG(it != idLookupMap_.end(),
                     "invalid shader id or shader has not yet been created");
  return shaderList_[it->second];
}

std::shared_ptr<GLShader> GLShaderManager::create(GLShader::ShaderType type,
                                                  const std::shared_ptr<File>& file,
                                                  GLShaderStatus requestedStatus) {

  std::shared_ptr<GLShader> shader = nullptr;
  auto it = fileLookupMap_.find(file);

  if(it != fileLookupMap_.end()) {
    shader = shaderList_[it->second];
  } else {
    shaderList_.emplace_back(std::make_shared<GLShader>(type, file, this));
    fileLookupMap_[file] = shaderList_.size() - 1;
    shader = shaderList_.back();
  }

  make(shader, requestedStatus);
  return shader;
}

} // namespace render

} // namespace sequoia
