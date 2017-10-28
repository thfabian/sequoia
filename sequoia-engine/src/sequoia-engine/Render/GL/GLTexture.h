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

#ifndef SEQUOIA_RENDER_GL_GLTEXTURE_H
#define SEQUOIA_RENDER_GL_GLTEXTURE_H

#include "sequoia-engine/Core/Hash.h"
#include "sequoia-engine/Render/GL/GLFwd.h"
#include "sequoia-engine/Render/Texture.h"

namespace sequoia {

namespace render {

/// @brief OpenGL texture implementation
/// @ingroup gl
class SEQUOIA_API GLTexture final : public Texture {
public:
  friend class GLTextureManager;

  GLTexture(const std::shared_ptr<Image>& image, const std::shared_ptr<TextureParameter>& param);
  ~GLTexture();

  /// @copydoc Texture::getImage
  virtual const std::shared_ptr<Image>& getImage() const override;

  /// @copydoc Texture::getParameter
  virtual const std::shared_ptr<TextureParameter>& getParameter() const override;

  /// @copydoc Texture::toString
  virtual std::string toString() const override;

  /// @copydoc Texture::hasImage
  virtual bool hasImage() const override;

  /// @brief Get the unique identifer of the texture
  ///
  /// Note that IDs might be reused after a texture has been destroyed.
  unsigned int getID() const;

  /// @brief Get the target of the texture
  GLenum getTarget() const;

  /// @brief Bind the texture to the current render pipline
  /// @note Do not call this function directly, use `GLStateCacheManager::bindTexture` instead.
  void bind();

  /// @brief Unbind texture
  void unbind();

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

  /// @brief Destroy the texture
  friend SEQUOIA_API void destroyGLTexture(GLTexture* texture) noexcept;

  SEQUOIA_GL_OBJECT(Texture)

protected:
  void makeValidImpl() override;

private:
  /// OpenGL texture index
  unsigned int id_;

  /// OpenGL texture target
  GLenum target_;

  /// Parameters of the texture
  std::shared_ptr<TextureParameter> param_;

  /// Texture size
  unsigned int width_, height_;

  /// Image used as basis of the texture (if any)
  std::shared_ptr<Image> image_;
};

} // render

} // namespace sequoia

SEQUOIA_DECLARE_STD_HASH(sequoia::render::GLTexture::Desc, desc, *desc.getParam(), *desc.getImage())

#endif
