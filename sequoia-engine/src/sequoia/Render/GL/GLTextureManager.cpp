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
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/GL/GLTextureManager.h"

namespace sequoia {

namespace render {

static GLenum getGLTarget(TextureParameter::TextureKind kind) {
  switch(kind) {
  case TextureParameter::TK_2D:
    return GL_TEXTURE_2D;
  case TextureParameter::TK_1D:
    return GL_TEXTURE_1D;
  case TextureParameter::TK_3D:
    return GL_TEXTURE_3D;
  case TextureParameter::TK_CubeMap:
    return GL_TEXTURE_CUBE_MAP;
  default:
    sequoia_unreachable("invalid TextureParameter::TextureKind");
  }
}

static GLenum getGLColorFormat(core::ColorFormat format) {
  switch(format) {
  case core::ColorFormat::RGB:
    return GL_RGB;
  case core::ColorFormat::BGR:
    return GL_BGR;
  case core::ColorFormat::RGBA:
    return GL_RGBA;
  case core::ColorFormat::BGRA:
    return GL_BGRA;
  default:
    sequoia_unreachable("invalid format");
  }
}

static GLenum getGLEdgeSampleKind(TextureParameter::EdgeSamplingKind format) {
  switch(format) {
  case TextureParameter::EK_Repeat:
    return GL_REPEAT;
  case TextureParameter::EK_MirroredRepeat:
    return GL_MIRRORED_REPEAT;
  case TextureParameter::EK_ClampToEdge:
    return GL_CLAMP_TO_EDGE;
  case TextureParameter::EK_ClampToBorder:
    return GL_CLAMP_TO_BORDER;
  default:
    sequoia_unreachable("invalid TextureParameter::EdgeSamplingKind");
  }
}

static GLenum getGLFilterKind(TextureParameter::FilterKind filter) {
  switch(filter) {
  case TextureParameter::FK_Nearest:
    return GL_NEAREST;
  case TextureParameter::FK_Linear:
    return GL_LINEAR;
  default:
    sequoia_unreachable("invalid TextureParameter::FilterKind");
  }
}

GLTextureManager::GLTextureManager(GLRenderer* renderer) : renderer_(renderer) {}

GLTextureManager::~GLTextureManager() {}

void GLTextureManager::make(const std::shared_ptr<GLTexture>& texture,
                            GLTextureStatus requestedStatus) {

  if(texture->status_ == GLTextureStatus::Created && requestedStatus != GLTextureStatus::Invalid)
    return;

  if(requestedStatus == GLTextureStatus::Invalid) {
    destroyGLTexture(texture.get());
    return;
  }

  if(texture->status_ == GLTextureStatus::Invalid) {
    glGenTextures(1, &texture->id_);
    texture->target_ = getGLTarget(texture->param_->Kind);

    if(texture->id_ == 0)
      SEQUOIA_THROW(RenderSystemException, "cannot create texture: '%s'",
                    texture->getImage()->getFile()->getPath());

    LOG(DEBUG) << "Created texture (ID=" << texture->id_ << ") from image \""
               << texture->getImage()->getFile()->getPath() << "\"";
    texture->status_ = GLTextureStatus::Created;
  }

  if(requestedStatus == GLTextureStatus::Created)
    return;

  if(texture->status_ == GLTextureStatus::Created) {
    LOG(DEBUG) << "Loading texture (ID=" << texture->id_ << ") ...";

    TextureParameter& param = *texture->getParameter();

    // TODO: Is it safe to always bind to unit 0 here?
    renderer_->getStateCacheManager()->bindTexture(0, texture.get());

    // Copy image
    switch(texture->param_->Kind) {
    case TextureParameter::TK_2D: {
      if(RegularImage* image = dyn_cast<RegularImage>(texture->getImage().get())) {
        glTexImage2D(texture->target_, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0,
                     getGLColorFormat(image->getColorFormat()), GL_UNSIGNED_BYTE,
                     image->getPixelData());
      } else {
      }
      break;
    }
    case TextureParameter::TK_1D:
    case TextureParameter::TK_3D:
    case TextureParameter::TK_CubeMap:
      sequoia_unreachable("not yet implemented");
    }

    // Set parameters
    glTexParameteri(texture->target_, GL_TEXTURE_WRAP_S,
                    getGLEdgeSampleKind(param.Dim1EdgeSampling));
    glTexParameteri(texture->target_, GL_TEXTURE_WRAP_T,
                    getGLEdgeSampleKind(param.Dim2EdgeSampling));
    glTexParameteri(texture->target_, GL_TEXTURE_MAG_FILTER, getGLFilterKind(param.MagFilter));

    if(param.UseMipmap) {
      // Generate mip-maps
      if(param.InterpolateBetweenMipmaps)
        glTexParameteri(texture->target_, GL_TEXTURE_MIN_FILTER,
                        param.MinFilter == TextureParameter::FK_Linear ? GL_LINEAR_MIPMAP_LINEAR
                                                                       : GL_LINEAR_MIPMAP_NEAREST);
      else
        glTexParameteri(texture->target_, GL_TEXTURE_MIN_FILTER,
                        param.MinFilter == TextureParameter::FK_Linear ? GL_NEAREST_MIPMAP_LINEAR
                                                                       : GL_NEAREST_MIPMAP_NEAREST);

      glGenerateMipmap(texture->target_);
    } else {
      glTexParameteri(texture->target_, GL_TEXTURE_MIN_FILTER, getGLFilterKind(param.MinFilter));
    }

    texture->status_ = GLTextureStatus::Loaded;
    
    // Unbind the texture. This is necessary to make sure when we bind it during rendering we also 
    // set the sampler of the program.
    renderer_->getStateCacheManager()->unbindTexture(0);    
    
    LOG(DEBUG) << "Successfully uploaded texture (ID=" << texture->id_ << ")";
  }
}

std::shared_ptr<GLTexture> GLTextureManager::create(const std::shared_ptr<Image>& image,
                                                    const std::shared_ptr<TextureParameter>& param,
                                                    GLTextureStatus requestedStatus) {
  std::shared_ptr<GLTexture> texture = nullptr;
  GLTexture::Desc desc(image, param);
  auto it = descLookupMap_.find(desc);

  if(it != descLookupMap_.end()) {
    texture = textureList_[it->second];
  } else {
    textureList_.emplace_back(std::make_shared<GLTexture>(image, param, renderer_));
    descLookupMap_[desc] = textureList_.size() - 1;
    texture = textureList_.back();
  }

  make(texture, requestedStatus);
  return texture;
}

} // render

} // namespace sequoia
