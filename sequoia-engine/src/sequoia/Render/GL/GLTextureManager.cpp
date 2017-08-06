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

#include "sequoia/Core/Casting.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/Exception.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLRenderer.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/GL/GLTextureManager.h"
#include <gli/gli.hpp>
#include <opencv2/opencv.hpp>

namespace sequoia {

namespace render {

namespace {

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

/// @brief Allocate an empty texture of size `width x height` of the given `internalFormat`
static void allocateTexture(GLenum target, int width, int height, GLenum internalFormat) {
  switch(target) {
  case GL_TEXTURE_2D:
    glTexImage2D(target, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    break;
  case GL_TEXTURE_1D:
  case GL_TEXTURE_3D:
  case GL_TEXTURE_CUBE_MAP:
  default:
    sequoia_unreachable("not yet implemented");
  }
}

/// @brief Upload regular image to device
///
/// https://stackoverflow.com/questions/16809833/opencv-image-loading-for-opengl-texture
static void uploadRegularImage(GLenum target, const RegularImage* image) {

  // TODO: this needs to be the StateCache manager of the RessourceThread
  GLStateCacheManager* manager = getGLRenderer().getStateCacheManager();

  // We need to vertically flip OpenCV images as OpenGL expects (0, 0) to be in the bottom left
  // corner and not in top left!
  cv::Mat imageFlipped;
  cv::flip(image->getMat(), imageFlipped, 0);

  // Set pixel storage parameter
  GLPixelFormat format = manager->getDefaultPixelFormat();
  format.set(GL_UNPACK_ALIGNMENT, imageFlipped.step[0] & 3 ? 1 : 4);
  format.set(GL_UNPACK_ROW_LENGTH, imageFlipped.step[0] / imageFlipped.elemSize());
  manager->setPixelFormat(format);

  switch(target) {
  case GL_TEXTURE_2D:
    glTexImage2D(target, 0, GL_RGBA, imageFlipped.cols, imageFlipped.rows, 0,
                 getGLColorFormat(image->getColorFormat()), GL_UNSIGNED_BYTE, imageFlipped.ptr());
    break;
  case GL_TEXTURE_1D:
  case GL_TEXTURE_3D:
  case GL_TEXTURE_CUBE_MAP:
  default:
    sequoia_unreachable("not yet implemented");
  }

  // Reset the pixel storage parameters
  manager->resetPixelFormat();
}

/// @brief Upload (compressed) texture image to device
static void uploadTextureImage(GLenum target, const TextureImage* image) {
  const gli::texture& gliTexture = image->getTexture();

  gli::gl GLTranslator(gli::gl::PROFILE_GL33);
  gli::gl::format format = GLTranslator.translate(gliTexture.format(), gliTexture.swizzles());

  GLenum texTarget = (GLenum)GLTranslator.translate(gliTexture.target());
  if(target != texTarget) {
    SEQUOIA_THROW(RenderSystemException,
                  "cannot create texture: '%s': expected target '%s', got '%s'",
                  image->getFile()->getPath(), target, texTarget);
  }

  glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, gliTexture.levels() - 1);
  glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzles[0]);
  glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzles[1]);
  glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzles[2]);
  glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzles[3]);

  auto extent = gliTexture.extent();
  int faceTotal = gliTexture.layers() * gliTexture.faces();

  switch(target) {
  case GL_TEXTURE_1D:
    glTexStorage1D(target, gliTexture.levels(), (GLenum)format.Internal, extent.x);
    break;
  case GL_TEXTURE_2D:
  case GL_TEXTURE_CUBE_MAP:
    glTexStorage2D(target, gliTexture.levels(), (GLenum)format.Internal, extent.x,
                   target == GL_TEXTURE_2D ? extent.y : faceTotal);
    break;
  case GL_TEXTURE_3D:
    glTexStorage3D(target, gliTexture.levels(), (GLenum)format.Internal, extent.x, extent.y,
                   extent.z);
    break;
  default:
    sequoia_unreachable("invalid texture target");
    break;
  }

  for(std::size_t layer = 0; layer < gliTexture.layers(); ++layer) {
    for(std::size_t face = 0; face < gliTexture.faces(); ++face) {
      for(std::size_t level = 0; level < gliTexture.levels(); ++level) {

        auto curExtent = gliTexture.extent(level);
        auto curTarget = gli::is_target_cube(gliTexture.target())
                             ? (GL_TEXTURE_CUBE_MAP_POSITIVE_X + face)
                             : target;

        switch(target) {
        case GL_TEXTURE_1D:
          if(gli::is_compressed(gliTexture.format()))
            glCompressedTexSubImage1D(curTarget, level, 0, curExtent.x, (GLenum)format.Internal,
                                      gliTexture.size(level), gliTexture.data(layer, face, level));
          else
            glTexSubImage1D(curTarget, level, 0, curExtent.x, (GLenum)format.External,
                            (GLenum)format.Type, gliTexture.data(layer, face, level));
          break;
        case GL_TEXTURE_2D:
        case GL_TEXTURE_CUBE_MAP:
          if(gli::is_compressed(gliTexture.format()))
            glCompressedTexSubImage2D(curTarget, level, 0, 0, curExtent.x, curExtent.y,
                                      (GLenum)format.Internal, gliTexture.size(level),
                                      gliTexture.data(layer, face, level));
          else
            glTexSubImage2D(curTarget, level, 0, 0, curExtent.x, curExtent.y,
                            (GLenum)format.External, (GLenum)format.Type,
                            gliTexture.data(layer, face, level));
          break;
        case GL_TEXTURE_3D:
          if(gli::is_compressed(gliTexture.format()))
            glCompressedTexSubImage3D(curTarget, level, 0, 0, 0, curExtent.x, curExtent.y,
                                      curExtent.z, (GLenum)format.Internal, gliTexture.size(level),
                                      gliTexture.data(layer, face, level));
          else
            glTexSubImage3D(curTarget, level, 0, 0, 0, curExtent.x, curExtent.y, curExtent.z,
                            (GLenum)format.External, (GLenum)format.Type,
                            gliTexture.data(layer, face, level));
          break;
        default:
          sequoia_unreachable("invalid texture target");
        }
      }
    }
  }
}

} // anonymous namespace

GLTextureManager::~GLTextureManager() {}

void GLTextureManager::makeValid(GLTexture* texture) {
  // TODO: for DSA use glCreateTextures
  glGenTextures(1, &texture->id_);
  texture->target_ = getGLTarget(texture->param_->Kind);

  if(texture->id_ == 0)
    SEQUOIA_THROW(RenderSystemException, "cannot create texture: '%s'",
                  texture->getImage()->getFile()->getPath());

  LOG(DEBUG) << "Created texture (ID=" << texture->id_ << ") from image \""
             << texture->getImage()->getFile()->getPath() << "\"";

  LOG(DEBUG) << "Loading texture (ID=" << texture->id_ << ") ...";

  TextureParameter& param = *texture->getParameter();

  // TODO: this needs to be the StateCache manager of the RessourceThread
  GLStateCacheManager* manager = getGLRenderer().getStateCacheManager();

  // Bind texture to unit 0 (doesn't really matter which unit we bind it to)
  manager->bindTexture(0, texture, true);

  if(texture->hasImage()) {
    // Uploade image to device
    if(RegularImage* regularImage = dyn_cast<RegularImage>(texture->getImage().get())) {
      uploadRegularImage(texture->target_, regularImage);
      if(param.UseMipmap)
        glGenerateMipmap(texture->target_);
    } else if(TextureImage* texImage = dyn_cast<TextureImage>(texture->getImage().get()))
      uploadTextureImage(texture->target_, texImage);
    else
      sequoia_unreachable("invalid image format");

  } else {
    // Allocate texture
    allocateTexture(texture->target_, texture->width_, texture->height_, GL_RGBA);
  }

  // Set interpolation parameters
  glTexParameteri(texture->target_, GL_TEXTURE_WRAP_S, getGLEdgeSampleKind(param.Dim1EdgeSampling));
  glTexParameteri(texture->target_, GL_TEXTURE_WRAP_T, getGLEdgeSampleKind(param.Dim2EdgeSampling));
  glTexParameteri(texture->target_, GL_TEXTURE_MAG_FILTER, getGLFilterKind(param.MagFilter));

  if(param.UseMipmap) {
    if(param.InterpolateBetweenMipmaps)
      glTexParameteri(texture->target_, GL_TEXTURE_MIN_FILTER,
                      param.MinFilter == TextureParameter::FK_Linear ? GL_LINEAR_MIPMAP_LINEAR
                                                                     : GL_LINEAR_MIPMAP_NEAREST);
    else
      glTexParameteri(texture->target_, GL_TEXTURE_MIN_FILTER,
                      param.MinFilter == TextureParameter::FK_Linear ? GL_NEAREST_MIPMAP_LINEAR
                                                                     : GL_NEAREST_MIPMAP_NEAREST);
  } else {
    glTexParameteri(texture->target_, GL_TEXTURE_MIN_FILTER, getGLFilterKind(param.MinFilter));
  }

  // Unbind the texture. This is necessary to make sure when we bind it during rendering we also
  // set the sampler of the program.
  manager->unbindTexture(0);

  LOG(DEBUG) << "Successfully uploaded texture (ID=" << texture->id_ << ")";
}

std::shared_ptr<GLTexture>
GLTextureManager::create(const std::shared_ptr<Image>& image,
                         const std::shared_ptr<TextureParameter>& param) {
  SEQUOIA_LOCK_GUARD(mutex_);

  GLTexture::Desc desc(image, param);
  auto it = descLookupMap_.find(desc);

  if(it != descLookupMap_.end())
    return textureList_[it->second];

  textureList_.emplace_back(std::make_shared<GLTexture>(image, param));
  descLookupMap_[desc] = textureList_.size() - 1;
  return textureList_.back();
}

void GLTextureManager::remove(const std::shared_ptr<GLTexture>& texture) noexcept {
  SEQUOIA_LOCK_GUARD(mutex_);
  textureList_.erase(std::remove(textureList_.begin(), textureList_.end(), texture),
                     textureList_.end());

  if(texture->hasImage()) {
    GLTexture::Desc desc(texture->getImage(), texture->getParameter());
    descLookupMap_.erase(desc);
  }
}

std::shared_ptr<Image> GLTextureManager::getTextureAsImage(GLTexture* texture) {
  if(texture->hasImage())
    return texture->getImage();

  return nullptr;
}

} // render

} // namespace sequoia
