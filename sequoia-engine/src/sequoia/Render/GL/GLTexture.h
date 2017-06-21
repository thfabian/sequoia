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

#include "sequoia/Render/Texture.h"

namespace sequoia {

namespace render {

class GLTexture final : public Texture {
  friend class GLTextureManager;

  /// Image used as basis of the texture
  std::shared_ptr<Image> image_;

  /// Parameter of the texture
  TextureParameter param_;

public:
  GLTexture(std::shared_ptr<Image> image, TextureParameter& param);
  ~GLTexture();

  /// @copydoc Texture::getImage
  const std::shared_ptr<Image>& getImage() const override;

  /// @copydoc Texture::getParameter
  const TextureParameter& getParameter() const override;

  /// @copydoc Texture::getLog
  std::string getLog() const override;

  /// @copydoc Texture::toString
  std::string toString() const override;

  SEQUOIA_GL_OBJECT(Texture)
};

} // render

} // namespace sequoia

#endif
