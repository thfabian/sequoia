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

#ifndef SEQUOIA_RENDER_GL_TEXTUREMANAGER_H
#define SEQUOIA_RENDER_GL_TEXTUREMANAGER_H

#include "sequoia/Core/HashCombine.h"
#include "sequoia/Render/GL/GLTexture.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace render {

/// @brief Manage OpenGL Textures by creating and loading them from source
///
/// A TextureManager is attached to a specific OpenGL context.
///
/// @ingroup gl
class SEQUOIA_API GLTextureManager : public NonCopyable {

  /// Record of all the registered shaders (use count of 1 implies the shader is *not* in use)
  std::vector<std::shared_ptr<GLTexture>> textureList_;

  /// Lookup map for shader ID
  std::unordered_map<unsigned int, std::size_t> idLookupMap_;

  /// Lookup map for files
  std::unordered_map<GLTexture::Desc, std::size_t> descLookupMap_;

public:
  /// @brief Destroy all remaining textures
  ~GLTextureManager();
};

} // namespace render

} // namespace sequoia

#endif
