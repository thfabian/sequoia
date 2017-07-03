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
#include "sequoia/Core/Format.h"
#include "sequoia/Core/Logging.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLFrameBufferObject.h"
#include "sequoia/Render/GL/GLRenderSystem.h"
#include "sequoia/Render/GL/GLStateCacheManager.h"
#include "sequoia/Render/GL/GLTexture.h"

namespace sequoia {

namespace render {

static void checkFramebufferStatus(unsigned int id) {
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if(status != GL_FRAMEBUFFER_COMPLETE) {
    switch(status) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      LOG(ERROR) << "Framebuffer (ID=" << id << ") incomplete: Attachment is NOT complete";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      LOG(ERROR) << "Framebuffer (ID=" << id << ") incomplete: No image is attached to FBO";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      LOG(ERROR) << "Framebuffer (ID=" << id << ") incomplete: Draw buffer";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      LOG(ERROR) << "Framebuffer (ID=" << id << ") incomplete: Read buffer";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      LOG(ERROR) << "Framebuffer (ID=" << id << ") incomplete: Multisample";
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      LOG(ERROR) << "Framebuffer (ID=" << id << ") incomplete: Unsupported by FBO implementation";
      break;
    default:
      LOG(ERROR) << "Framebuffer (ID=" << id << ") incomplete: Unknown error";
      break;
    }
  }
}

GLFrameBufferObject::GLFrameBufferObject(const FrameBufferObjectParameter& param)
    : FrameBufferObject(RK_OpenGL), fboID_(0), rboDepthID_(0), rboColorID_(0), param_(param) {
  glGenFramebuffers(1, &fboID_);
}

GLFrameBufferObject::~GLFrameBufferObject() {
  if(rboDepthID_ != 0)
    glDeleteRenderbuffers(1, &rboDepthID_);

  if(rboColorID_ != 0)
    glDeleteRenderbuffers(1, &rboColorID_);

  glDeleteFramebuffers(1, &fboID_);
}

unsigned int GLFrameBufferObject::getID() const { return fboID_; }

const FrameBufferObjectParameter& GLFrameBufferObject::getParam() const { return param_; }

void GLFrameBufferObject::attachTexture(GLFragmentData::Data attachmentPoint,
                                        const std::shared_ptr<GLTexture>& texture) {
  getGLRenderSystem().getStateCacheManager()->bindFrameBufferObject(this);

  // Generate the depth-buffer
  if(rboDepthID_ == 0) {
    glGenRenderbuffers(1, &rboDepthID_);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthID_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, param_.Width, param_.Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Attach depth buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepthID_);
  }

  // TODO: Check for consistency of the texture

  // Attach the texture
  SEQUOIA_ASSERT_MSG(texture->getTarget() != GL_TEXTURE_2D, "only 2D textures are supported (yet)");
  glFramebufferTexture2D(GL_FRAMEBUFFER, GLFragmentData::getAttachment(attachmentPoint),
                         texture->getTarget(), texture->getID(), 0);

  // Update fragment data map
  fragmentDataMap_.emplace(static_cast<unsigned int>(attachmentPoint), texture);

  // Update the draw-buffers (`drawBuffer[loc] = attachment` means the output fragment data at
  // location `loc` is written to the render buffer associated with `attachment`)
  std::vector<GLenum> drawBuffers = getDrawBuffers();
  glDrawBuffers(drawBuffers.size(), drawBuffers.data());

  checkFramebufferStatus(fboID_);
  getGLRenderSystem().getStateCacheManager()->unbindFrameBufferObject();
}

void GLFrameBufferObject::bind() { glBindFramebuffer(GL_FRAMEBUFFER, fboID_); }

void GLFrameBufferObject::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

std::string GLFrameBufferObject::toString() const {
  return core::format(
      "GLFrameBufferObject[\n"
      "  fboID = %s,\n"
      "  rboDepthID = %s,\n"
      "  rboColorID = %s,\n"
      "  param = %s,\n"
      "  fragmentDataMap = %s\n"
      "]",
      fboID_, rboDepthID_ == 0 ? "invalid" : std::to_string(rboDepthID_),
      rboColorID_ == 0 ? "invalid" : std::to_string(rboColorID_), core::indent(param_.toString()),
      !fragmentDataMap_.empty()
          ? core::indent(core::toStringRange(fragmentDataMap_,
                                             [](const auto& pair) {
                                               std::stringstream ss;
                                               ss << "[\n  fragment_data = " << pair.second << "\n"
                                                  << "  texture = "
                                                  << core::indent(pair.second->toString()) << "\n"
                                                  << "]";
                                               return ss.str();
                                             }))
          : "null");
}

std::vector<GLenum> GLFrameBufferObject::getDrawBuffers() const noexcept {
  // The default of the draw buffer is `GL_NONE` so we only need to fill the buffer up to the
  // highest `location` value
  unsigned int maxLocation = std::accumulate(
      fragmentDataMap_.begin(), fragmentDataMap_.end(), 0,
      [](unsigned int maxLocation, const auto& pair) { return std::max(maxLocation, pair.first); });

  std::vector<GLenum> drawBuffers(maxLocation + 1, GL_NONE);
  std::for_each(fragmentDataMap_.begin(), fragmentDataMap_.end(), [&drawBuffers](const auto& pair) {
    drawBuffers[pair.first] = GLFragmentData::getAttachment(pair.first);
  });

  return drawBuffers;
}

} // namespace render

} // namespace sequoia
