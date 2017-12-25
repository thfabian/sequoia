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

#ifndef SEQUOIA_ENGINE_RENDER_NULL_NULLTEXTURE_H
#define SEQUOIA_ENGINE_RENDER_NULL_NULLTEXTURE_H

#include "sequoia-engine/Render/Texture.h"

namespace sequoia {

namespace render {

/// @brief Null texture implementation
/// @ingroup null
class SEQUOIA_API NullTexture final : public Texture {

  /// Image used as basis of the texture (if any)
  std::shared_ptr<Image> image_;

  /// Parameters of the texture
  std::shared_ptr<TextureParameter> param_;

public:
  NullTexture(const std::shared_ptr<Image>& image, const std::shared_ptr<TextureParameter>& param);
  ~NullTexture();

  /// @copydoc Texture::getImage
  virtual const std::shared_ptr<Image>& getImage() const override;

  /// @copydoc Check if the texture was constructed from an Image
  virtual bool hasImage() const override;

  /// @copydoc Texture::getParameter
  const std::shared_ptr<TextureParameter>& getParameter() const override;

  /// @copydoc Texture::toString
  virtual std::string toString() const override;

  SEQUOIA_NULL_OBJECT(Texture)

protected:
  void makeValidImpl() override;
};

} // namespace render

} // namespace sequoia

#endif
