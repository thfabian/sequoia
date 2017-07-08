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

#ifndef SEQUOIA_RENDER_GL_GLTEXTUREMANAGER_H
#define SEQUOIA_RENDER_GL_GLTEXTUREMANAGER_H

#include "sequoia/Render/GL/GLTexture.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace sequoia {

namespace render {

/// @brief Manage OpenGL Textures by creating, allocating and loading theme
///
/// A TextureManager is attached to a specific OpenGL context. Note that texture are not necessarily
/// tied to images and have lots of other areas applications.
///
/// @ingroup gl
class SEQUOIA_API GLTextureManager : public NonCopyable {

  /// Record of all the registered shaders (use count of 1 implies the shader is *not* in use)
  std::vector<std::shared_ptr<GLTexture>> textureList_;

  /// Lookup map for texture which were loaded from an image (using the hash of the texture
  /// description i.e Parameter + Image)
  std::unordered_map<GLTexture::Desc, std::size_t> descLookupMap_;

  /// Associated Renderer
  GLRenderer* renderer_;

public:
  /// @brief Destroy all remaining textures
  ~GLTextureManager();

  /// @brief Create a textrue from `image` and `param`
  ///
  /// @param image            Image used as a basis of the texture
  /// @param param            Parameter used for initialization
  /// @param requestedStatus  Requested target status
  /// @throws RenderSystemException
  std::shared_ptr<GLTexture> create(const std::shared_ptr<Image>& image,
                                    const std::shared_ptr<TextureParameter>& param,
                                    GLTextureStatus requestedStatus = GLTextureStatus::Loaded);

  /// @brief Convert the texture to `status`
  /// @throws RenderSystemException
  void make(const std::shared_ptr<GLTexture>& texture, GLTextureStatus requestedStatus);

  /// @brief Convert the texture to `GLProgramStatus::Loaded` which makes it usable in the render
  /// pipeline
  /// @see GLTextureManager::make
  void makeValid(const std::shared_ptr<GLTexture>& texture) {
    make(texture, GLTextureStatus::Loaded);
  }

  /// @brief Get a copy of the `texture` as an `image`
  std::shared_ptr<Image> getTextureAsImage(const std::shared_ptr<GLTexture>& texture);

  /// @brief Get the texture by OpenGL shader `id`
  const std::shared_ptr<GLTexture>& get(unsigned int id) const;
};

} // namespace render

} // namespace sequoia

#endif
