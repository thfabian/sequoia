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

#ifndef SEQUOIA_ENGINE_RENDER_SHADERSOURCEMANAGER_H
#define SEQUOIA_ENGINE_RENDER_SHADERSOURCEMANAGER_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include <string>
#include <unordered_map>

namespace sequoia {

namespace render {

/// @brief Manager of shader sources
/// @ingroup render
class SEQUOIA_API ShaderSourceManager : public NonCopyable {
public:
  enum ShaderLanguage { GLSL = 0 };

  /// @brief Set the source of shader lanaguage
  ShaderSourceManager(ShaderLanguage language = GLSL);

  /// @brief Get the source of the shader `filename`
  ///
  /// @throws RenderSystemException   Shader `filename` does not exists.
  const char* load(const std::string& filename) const;

  /// @brief Check if shader `filename` exists
  bool hasSource(const std::string& filename) const noexcept;

private:
  /// Cached shader sources
  std::unordered_map<std::string, std::string> sources_;
};

} // namespace render

} // namespace sequoia

#endif
