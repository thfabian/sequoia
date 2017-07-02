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

#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLTexture.h"
#include "sequoia/Render/GL/GLTextureManager.h"

namespace sequoia {

namespace render {

static const char* statusToString(GLTextureStatus status) {
  switch(status) {
  case GLTextureStatus::Invalid:
    return "Invalid";
  case GLTextureStatus::Created:
    return "Created";
  case GLTextureStatus::Loaded:
    return "Loaded";
  default:
    sequoia_unreachable("invalid GLTextureStatus");
  }
}

GLTexture::GLTexture(std::shared_ptr<Image> image, const std::shared_ptr<TextureParameter>& param,
                     GLRenderer* renderer)
    : Texture(RK_OpenGL), status_(GLTextureStatus::Invalid), id_(0), target_(GL_INVALID_ENUM),
      image_(image), param_(param), renderer_(renderer) {}

GLTexture::~GLTexture() { destroyGLTexture(this); }

const std::shared_ptr<Image>& GLTexture::getImage() const { return image_; }

const std::shared_ptr<TextureParameter>& GLTexture::getParameter() const { return param_; }

bool GLTexture::isValid() const { return (status_ == GLTextureStatus::Loaded); }

unsigned int GLTexture::getID() const { return id_; }

GLenum GLTexture::getTarget() const { return target_; }

GLTextureStatus GLTexture::getStatus() const { return status_; }

void GLTexture::bind() {
  if(!isValid())
    renderer_->getTextureManager()->makeValid(dyn_pointer_cast<GLTexture>(shared_from_this()));
  glBindTexture(target_, id_);
}

void GLTexture::unbind() { glBindTexture(target_, 0); }

std::string GLTexture::toString() const {
  return core::format("GLTexture[\n"
                      "  id = %i,\n"
                      "  status = %s,\n"
                      "  image = %s,\n"
                      "  param = %s\n"
                      "]",
                      id_, statusToString(status_), core::indent(image_->toString()),
                      core::indent(param_->toString()));
}

void destroyGLTexture(GLTexture* texture) noexcept {
  if(texture->status_ == GLTextureStatus::Invalid)
    return;

  LOG(DEBUG) << "Deleting texture (ID=" << texture->id_ << ")";
  glDeleteTextures(1, &texture->id_);
  texture->id_ = 0;
  texture->status_ = GLTextureStatus::Invalid;
}

} // render

} // namespace sequoia
