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

#include "sequoia/Core/Byte.h"
#include "sequoia/Core/Format.h"
#include "sequoia/Core/StringUtil.h"
#include "sequoia/Core/Unreachable.h"
#include "sequoia/Render/GL/GL.h"
#include "sequoia/Render/GL/GLBuffer.h"
#include <cstring>

#include <iostream>

namespace sequoia {

namespace render {

static GLenum getGLBufferUsage(Buffer::UsageHint hint) {
  switch(hint) {
  case Buffer::UH_Static:
  case Buffer::UH_StaticWriteOnly:
    return GL_STATIC_DRAW;
  case Buffer::UH_Dynamic:
  case Buffer::UH_DynamicWriteOnly:
    return GL_DYNAMIC_DRAW;
  case Buffer::UH_DynamicWriteOnlyDiscardable:
    return GL_STREAM_DRAW;
  default:
    return GL_DYNAMIC_DRAW;
  }
}

GLBuffer::GLBuffer(GLenum target, int numBuffers)
    : modifyBufferIdx_(0), target_(target), hint_(GL_INVALID_ENUM), numBytes_(0), isLocked_(false) {
  bufferIds_.resize(numBuffers);
  glCreateBuffers(bufferIds_.size(), bufferIds_.data());
}

GLBuffer::~GLBuffer() { glDeleteBuffers(bufferIds_.size(), bufferIds_.data()); }

void GLBuffer::bind(GLBuffer::BindKind kind) {
  switch(kind) {
  case BK_Draw:
    glBindBuffer(target_, bufferIds_[getDrawBufferIdx()]);
    break;
  case BK_Modify:
    glBindBuffer(target_, bufferIds_[getModifyBufferIdx()]);
    break;
  default:
    sequoia_unreachable("invalid BindKind");
  }
}

void GLBuffer::unbind(GLBuffer::BindKind kind) { glBindBuffer(target_, 0); }

void* GLBuffer::lock(Buffer::LockOption option) {
  SEQUOIA_ASSERT_MSG(!isLocked(), "buffer already locked");
  isLocked_ = true;

  // Bind the modify buffer
  bind(BK_Modify);

  // Discard the buffer if requested
  if(option == Buffer::LO_Discard)
    glBufferData(target_, numBytes_, nullptr, hint_);

  GLenum access = GL_WRITE_ONLY;
  switch(option) {
  case Buffer::LO_Normal:
    access = GL_READ_WRITE;
    break;
  case Buffer::LO_ReadOnly:
    access = GL_READ_ONLY;
    break;
  default: // LO_Discard, LO_WriteOnly
    access = GL_WRITE_ONLY;
  }

  // Initiate the DMA
  return glMapBuffer(target_, access);
}

void GLBuffer::unlock() {
  SEQUOIA_ASSERT_MSG(isLocked(), "buffer not locked");
  isLocked_ = false;

  // Bind the modify buffer
  bind(BK_Modify);

  // Stop the DMA
  glUnmapBuffer(target_);
}

void GLBuffer::allocate(std::size_t numBytes, Buffer::UsageHint hint) {
  numBytes_ = numBytes;
  hint_ = getGLBufferUsage(hint);
  for(unsigned int id : bufferIds_) {
    glBindBuffer(target_, id);
    glBufferData(target_, numBytes_, nullptr, hint_);
  }
}

void GLBuffer::write(const void* src, std::size_t offset, std::size_t length, bool discardBuffer) {
  bind(BK_Modify);

  if(discardBuffer) 
    glBufferData(target_, numBytes_, nullptr, hint_);
  
  void* dest = glMapBuffer(target_, GL_WRITE_ONLY);
  std::memcpy(static_cast<void*>(((Byte*)dest + offset)), src, length);
  glUnmapBuffer(target_);
}

void GLBuffer::read(std::size_t offset, std::size_t length, void* dest) {
  bind(BK_Modify);

  void* src = glMapBuffer(target_, GL_READ_ONLY);
  std::memcpy(dest, static_cast<void*>(((Byte*)src + offset)), length);
  glUnmapBuffer(target_);
}

void GLBuffer::nextTimestep() { modifyBufferIdx_ = (modifyBufferIdx_ + 1) % bufferIds_.size(); }

unsigned int GLBuffer::getDrawBufferIdx() const {
  return (modifyBufferIdx_ + (bufferIds_.size() - 1)) % bufferIds_.size();
}

unsigned int GLBuffer::getModifyBufferIdx() const { return modifyBufferIdx_; }

std::string GLBuffer::toString() const {
  return core::format("GLBuffer[\n"
                      "  bufferIds = %s,\n"
                      "  target = %s,\n"
                      "  hint = %s,\n"
                      "  numBytes = %s\n"
                      "]",
                      core::RangeToString()(bufferIds_), target_, hint_, numBytes_);
}

} // namespace render

} // namespace sequoia
