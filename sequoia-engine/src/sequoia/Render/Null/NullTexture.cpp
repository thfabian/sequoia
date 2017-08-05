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

#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Render/Null/NullTexture.h"

namespace sequoia {

namespace render {

NullTexture::NullTexture(const std::shared_ptr<Image>& image,
                         const std::shared_ptr<TextureParameter>& param)
    : Texture(RK_Null), image_(image), param_(param) {}

NullTexture::~NullTexture() {}

const std::shared_ptr<Image>& NullTexture::getImage() const { return image_; }

bool NullTexture::hasImage() const { return (image_ != nullptr); }

const std::shared_ptr<TextureParameter>& NullTexture::getParameter() const { return param_; }

void NullTexture::makeValidImpl() {}

std::string NullTexture::toString() const {
  return core::format("NullTexture[\n"
                      "  image = %s,\n"
                      "  param = %s\n"
                      "]",
                      image_ ? core::indent(image_->toString()) : "null",
                      core::indent(param_->toString()));
}

} // namespace render

} // namespace sequoia
