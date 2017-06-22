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
#include "sequoia/Render/Texture.h"

namespace sequoia {

namespace render {

class GLTexture final : public Texture {
  friend class GLTextureManager;

  /// Image used as basis of the texture
  std::shared_ptr<Image> image_;

  /// Parameter of the texture
  std::shared_ptr<TextureParameter> param_;

public:
  GLTexture(std::shared_ptr<Image> image, const std::shared_ptr<TextureParameter>& param);
  ~GLTexture();

  /// @copydoc Texture::getImage
  const std::shared_ptr<Image>& getImage() const override;

  /// @copydoc Texture::getParameter
  const std::shared_ptr<TextureParameter>& getParameter() const override;

  /// @copydoc Texture::getLog
  std::string getLog() const override;

  /// @copydoc Texture::toString
  std::string toString() const override;

  /// @brief Texture description (image + parameter)
  class Desc {
    std::shared_ptr<Image> image_;
    std::shared_ptr<TextureParameter> param_;

  public:
    Desc(const std::shared_ptr<Image>& image, const std::shared_ptr<TextureParameter>& param)
        : image_(image), param_(param) {}

    const std::shared_ptr<Image>& getImage() const { return image_; }
    const std::shared_ptr<TextureParameter>& getParam() const { return param_; }

    inline bool operator==(const Desc& other) const noexcept {
      return image_ == other.image_ && param_ == other.param_;
    }
    inline bool operator!=(const Desc& other) const noexcept { return !(*this == other); }
  };

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
