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

#ifndef SEQUOIA_RENDER_GL_GLTEXTURE_H
#define SEQUOIA_RENDER_GL_GLTEXTURE_H

#include "sequoia/Core/HashCombine.h"
#include "sequoia/Render/GL/GLFwd.h"
#include "sequoia/Render/Texture.h"

namespace sequoia {

namespace render {

class GLRenderer;

/// @brief Status of an OpenGL program
/// @ingroup gl
enum class GLTextureStatus {
  Invalid = 0,
  Created, ///< Texture was registered within OpenGL and recieved a unique ID
  Loaded,  ///< Texture was successfully uploaded to the device
};

class SEQUOIA_API GLTexture final : public Texture {
  friend class GLTextureManager;

  /// Status of the texture
  GLTextureStatus status_;

  /// OpenGL texture index
  unsigned int id_;

  /// OpenGL texture target
  GLenum target_;

  /// Image used as basis of the texture
  std::shared_ptr<Image> image_;

  /// Parameters of the texture
  std::shared_ptr<TextureParameter> param_;

  /// Associated Renderer
  GLRenderer* renderer_;

public:
  GLTexture(std::shared_ptr<Image> image, const std::shared_ptr<TextureParameter>& param,
            GLRenderer* renderer);
  ~GLTexture();

  /// @copydoc Texture::getImage
  const std::shared_ptr<Image>& getImage() const override;

  /// @copydoc Texture::getParameter
  const std::shared_ptr<TextureParameter>& getParameter() const override;

  /// @brief Check if the program is valid i.e can be installed into the render pipeline
  bool isValid() const;

  /// @brief Get the unique identifer of the texture
  ///
  /// Note that IDs might be reused after a texture has been destroyed.
  unsigned int getID() const;

  /// @brief Get the status of the texture
  GLTextureStatus getStatus() const;

  /// @brief Bind the texture to the current render pipline
  /// @note Do not call this function directly, use `GLRenderer::bindTexture` instead.
  void bind();
  
  /// @brief Unbind texture
  void unbind();

  /// @copydoc Texture::toString
  std::string toString() const override;

  /// @brief Texture description (Image + TextureParameter)
  ///
  /// This is used for efficient hashing of textures.
  class Desc {
    std::shared_ptr<Image> image_;
    std::shared_ptr<TextureParameter> param_;

  public:
    Desc(const std::shared_ptr<Image>& image, const std::shared_ptr<TextureParameter>& param)
        : image_(image), param_(param) {}

    Desc(const Desc&) = default;
    Desc(Desc&&) = default;

    const std::shared_ptr<Image>& getImage() const { return image_; }
    const std::shared_ptr<TextureParameter>& getParam() const { return param_; }

    inline bool operator==(const Desc& other) const noexcept {
      return *image_ == *other.image_ && *param_ == *other.param_;
    }
    inline bool operator!=(const Desc& other) const noexcept { return !(*this == other); }
  };

  /// @brief Destroy the program
  friend SEQUOIA_API void destroyGLTexture(GLTexture* texture) noexcept;

  SEQUOIA_GL_OBJECT(Texture)
};

} // render

} // namespace sequoia

namespace std {

template <>
struct hash<sequoia::render::GLTexture::Desc> {
  std::size_t operator()(const sequoia::render::GLTexture::Desc& desc) const {
    std::size_t seed = 0;
    sequoia::core::hashCombine(seed,
                               std::hash<sequoia::render::TextureParameter>()(*desc.getParam()),
                               std::hash<sequoia::core::Image>()(*desc.getImage()));
    return seed;
  }
};

} // namespace std

#endif
