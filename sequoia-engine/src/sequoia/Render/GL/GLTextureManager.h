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

/// @brief Manage OpenGL textures by creating, allocating and loading them
///
/// Note that texture are not necessarily tied to images and have lots of other areas applications.
///
/// @ingroup gl
class SEQUOIA_API GLTextureManager : public NonCopyable {
  /// Access mutex
  SpinMutex mutex_;

  /// Record of all the registered textures (use count of 1 implies the texture is *not* in use)
  std::vector<std::shared_ptr<GLTexture>> textureList_;

  /// Lookup map for texture which were loaded from an image (using the hash of the texture
  /// description i.e Parameter + Image)
  std::unordered_map<GLTexture::Desc, std::size_t> descLookupMap_;

public:
  /// @brief Destroy all remaining textures
  ~GLTextureManager();

  /// @brief Create an *empty* textrue from `image` and `param`
  ///
  /// @param image  Image used as a basis of the texture
  /// @param param  Parameter used for initialization
  /// @returns Newly created texture which is *not* valid, call GLTexture::makeValid() to convert 
  ///          it into a valid state
  ///
  /// @remark Thread-safe
  std::shared_ptr<GLTexture> create(const std::shared_ptr<Image>& image,
                                    const std::shared_ptr<TextureParameter>& param);

  /// @brief Make the texture valid
  /// @throws RenderSystemExcption  Failed to initialize the texture
  void makeValid(GLTexture* texture);
  
  /// @brief Remove the `texture` (do nothing if the texute does not exist)
  void remove(const std::shared_ptr<GLTexture>& texture) noexcept;

  /// @brief Get a copy of the `texture` as an `image`
  std::shared_ptr<Image> getTextureAsImage(GLTexture* texture);
};

} // namespace render

} // namespace sequoia

#endif
