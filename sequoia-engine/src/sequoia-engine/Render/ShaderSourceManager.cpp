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

#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/Exception.h"
#include "sequoia-engine/Render/ShaderSourceManager.h"

namespace sequoia {

namespace render {

ShaderSourceManager::ShaderSourceManager(ShaderLanguage language) {
  switch(language) {
  case GLSL:
#define SHADER(Filename, Source) sources_[Filename] = Source;
#include "sequoia-engine/Render/Shaders/GLSL/Shaders.h"
#undef SHADER
    break;
  default:
    sequoia_unreachable("invalid ShaderLanguage");
  }
}

const char* ShaderSourceManager::load(const std::string& filename) const {
  auto it = sources_.find(filename);
  if(it == sources_.end())
    SEQUOIA_THROW(RenderSystemException, "shader {} does not exist", filename);
  return it->second.c_str();
}

bool ShaderSourceManager::hasSource(const std::string& filename) const noexcept {
  return sources_.count(filename);
}

} // namespace render

} // namespace sequoia
