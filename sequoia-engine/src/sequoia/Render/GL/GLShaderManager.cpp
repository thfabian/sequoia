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

GLShader* GLShaderManager::makeValid(std::unique_ptr<ShaderRecord>& record) {

  if(record->Status == ShaderStatus::OnDisk) {
    std::ifstream file(record->Path);

    if(!file.is_open())
      SEQUOIA_THROW(RenderSystemException, PLATFORM_STR("cannot load shader source: '%s'"),
                    record->Path.c_str());

    std::stringstream ss;
    ss << file.rdbuf();
    record->Code = ss.str();
    record->Status++;
  }

  if(record->Status == ShaderStatus::InMemory) {
    // Remove from map if exists

    record->Status++;
  }

  if(record->Status == ShaderStatus::Created) {

    record->Status++;
  }

  return record->Shader.get();
}

GLShader* GLShaderManager::create(const platform::String& path) {
  auto it = pathLookupMap_.find(path);

  // Check if shader already exists
  if(it != pathLookupMap_.end())
    return makeValid(shaderRecordMap_[it->second]);

  // Load and compile the shader from source
  auto record = std::make_unique<ShaderRecord>();
  record->Path = path;
  return makeValid(record);
}

} // namespace render

} // namespace sequoia
