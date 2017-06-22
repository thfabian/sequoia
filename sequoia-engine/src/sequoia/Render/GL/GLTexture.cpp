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

#include "sequoia/Core/Format.h"
#include "sequoia/Render/GL/GLTexture.h"

namespace sequoia {

namespace render {

GLTexture::GLTexture(std::shared_ptr<Image> image, const std::shared_ptr<TextureParameter>& param)
    : Texture(RK_OpenGL), image_(image), param_(param) {}

GLTexture::~GLTexture() {}

const std::shared_ptr<Image>& GLTexture::getImage() const { return image_; }

const std::shared_ptr<TextureParameter>& GLTexture::getParameter() const { return param_; }

std::string GLTexture::getLog() const { return std::string(); }

std::string GLTexture::toString() const {
  return core::format("GLTexture[\n"
                      " image = %s,\n"
                      " param = %s"
                      "]",
                      image_->toString(), param_->toString());
}

} // render

} // namespace sequoia
