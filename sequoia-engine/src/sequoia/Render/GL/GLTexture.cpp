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

#include "sequoia/Render/GL/GL.h"
#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLTexture.h"
#include "sequoia/Render/GL/GLTextureManager.h"

namespace sequoia {

namespace render {

GLTexture::GLTexture(const std::shared_ptr<Image>& image,
                     const std::shared_ptr<TextureParameter>& param)
    : Texture(RK_OpenGL), id_(0), target_(GL_INVALID_ENUM), param_(param), image_(image) {
  width_ = image->getWidth();
  height_ = image->getHeight();
}

GLTexture::~GLTexture() { destroyGLTexture(this); }

const std::shared_ptr<Image>& GLTexture::getImage() const { return image_; }

const std::shared_ptr<TextureParameter>& GLTexture::getParameter() const { return param_; }

std::string GLTexture::toString() const {
  return core::format("GLTexture[\n"
                      "  valid = %s\n"
                      "  id = %i,\n"
                      "  image = %s,\n"
                      "  param = %s\n"
                      "]",
                      isValid() ? "true" : "false", id_,
                      image_ ? core::indent(image_->toString()) : "null",
                      core::indent(param_->toString()));
}

bool GLTexture::hasImage() const { return image_ != nullptr; }

unsigned int GLTexture::getID() const { return id_; }

GLenum GLTexture::getTarget() const { return target_; }

void GLTexture::bind() { glBindTexture(target_, id_); }

void GLTexture::unbind() { glBindTexture(target_, 0); }

void GLTexture::makeValidImpl() { getGLRenderer().getTextureManager()->makeValid(this); }

void destroyGLTexture(GLTexture* texture) noexcept {
  if(!texture->isValid())
    return;

  LOG(DEBUG) << "Deleting texture (ID=" << texture->id_ << ")";
  glDeleteTextures(1, &texture->id_);
  texture->id_ = 0;
}

} // render

} // namespace sequoia
