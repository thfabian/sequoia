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

#include "sequoia/Core/UtfString.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GLShaderManager.h"
#include <fstream>
#include <sstream>

namespace sequoia {

namespace render {

void GLShaderManager::make(GLShader* shader, GLShaderStatus status) {
  if(status == GLShaderStatus::Compiled || status == GLShaderStatus::OnDisk)
    return;

  if(shader->status_ == GLShaderStatus::OnDisk) {
    std::ifstream file(shader->path_);

    if(!file.is_open())
      SEQUOIA_THROW(RenderSystemException, PLATFORM_STR("cannot load shader source: '%s'"),
                    shader->path_.c_str());

    std::stringstream ss;
    ss << file.rdbuf();
    shader->code_ = ss.str();
    shader->status_++;
  }

  if(status == GLShaderStatus::InMemory)
    return;

  if(shader->status_ == GLShaderStatus::InMemory) {
    // 0 means it has not yet been registered
    if(shader->id_ != 0)
      idLookupMap_.erase(shader->id_);

    shader->id_ = glCreateShader(GLShader::getGLShaderType(shader->type_));
    idLookupMap_.emplace(shader->id_, pathLookupMap_[shader->path_]);

    shader->status_++;
  }

  if(status == GLShaderStatus::Created)
    return;

  if(shader->status_ == GLShaderStatus::Created) {
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

      SEQUOIA_THROW(RenderSystemException, "failed to compile shader: '%s'", errorMessage.data());
    }

    shader->status_++;
  }
}

GLShader* GLShaderManager::get(unsigned int id) const {
  auto it = idLookupMap_.find(id);
  SEQUOIA_ASSERT_MSG(it != idLookupMap_.end(),
                     "invalid shader id or shader has not yet been created");
  return shaderList_[it->second].get();
}

GLShader* GLShaderManager::create(GLShader::ShaderType type, const platform::String& path,
                                  GLShaderStatus requestedStatus) {
  GLShader* shader = nullptr;
  auto it = pathLookupMap_.find(path);

  if(it != pathLookupMap_.end())
    shader = shaderList_[it->second].get();
  else {
    shaderList_.emplace_back(std::make_unique<GLShader>(type, path));
    pathLookupMap_[path] = shaderList_.size() - 1;
    shader = shaderList_.back().get();
  }

  make(shader, requestedStatus);
  return shader;
}

} // namespace render

} // namespace sequoia
