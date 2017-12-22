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

#include "sequoia-engine/Core/Byte.h"
#include "sequoia-engine/Core/Format.h"
#include "sequoia-engine/Core/StringUtil.h"
#include "sequoia-engine/Core/Unreachable.h"
#include "sequoia-engine/Render/GL/GL.h"
#include "sequoia-engine/Render/GL/GLBuffer.h"
#include <cstring>

// TODO: convert everything to DSA

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

GLBuffer::GLBuffer(GLenum target)
    : id_(0), target_(target), hint_(GL_INVALID_ENUM), numBytes_(0), isLocked_(false) {

  // TODO: for DSA replace with glCreateBuffers
  glGenBuffers(1, &id_);
}

GLBuffer::~GLBuffer() { glDeleteBuffers(1, &id_); }

void GLBuffer::bind() { glBindBuffer(target_, id_); }

void GLBuffer::unbind() { glBindBuffer(target_, 0); }

void* GLBuffer::lock(Buffer::LockOption option) {
  SEQUOIA_ASSERT_MSG(!isLocked(), "buffer already locked");
  isLocked_ = true;
  
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

  // TODO: allow returning a scratchpad memory for small buffers

  // Initiate the DMA
  bind();  
  return glMapBuffer(target_, access);
}

void GLBuffer::unlock() {
  SEQUOIA_ASSERT_MSG(isLocked(), "buffer not locked");
  isLocked_ = false;

  glUnmapBuffer(target_);
  unbind();    
}

void GLBuffer::allocate(std::size_t numBytes, Buffer::UsageHint hint) {
  numBytes_ = numBytes;
  hint_ = getGLBufferUsage(hint);
  bind();
  glBufferData(target_, numBytes_, nullptr, hint_);
  unbind();
}

void GLBuffer::write(const void* src, std::size_t offset, std::size_t length, bool discardBuffer) {
  SEQUOIA_ASSERT_MSG((offset + length) <= numBytes_, "out of bound writing");
  
  bind();

  if(discardBuffer)
    glBufferData(target_, numBytes_, nullptr, hint_);

  // TODO: figure out when using glBufferData vs. glMapBuffer

  void* dest = glMapBuffer(target_, GL_WRITE_ONLY);
  std::memcpy(static_cast<void*>(((Byte*)dest + offset)), src, length);
  glUnmapBuffer(target_);
  
  unbind(); 
}

void GLBuffer::read(std::size_t offset, std::size_t length, void* dest) {
  SEQUOIA_ASSERT_MSG((offset + length) <= numBytes_, "out of bound reading");

  bind();
  
  // TODO: figure out when using glGetNamedBufferSubData vs. glMapBuffer

  void* src = glMapBuffer(target_, GL_READ_ONLY);
  std::memcpy(dest, static_cast<void*>(((Byte*)src + offset)), length);
  glUnmapBuffer(target_);
  
  unbind();
}

unsigned int GLBuffer::getID() const { return id_; }

std::string GLBuffer::toString() const {
  return core::format("GLBuffer[\n"
                      "  id = {},\n"
                      "  target = {},\n"
                      "  hint = {},\n"
                      "  numBytes = {}\n"
                      "]",
                      id_, target_, hint_, numBytes_);
}

} // namespace render

} // namespace sequoia
