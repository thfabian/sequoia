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

#ifndef SEQUOIA_ENGINE_RENDER_GL_GLBUFFER_H
#define SEQUOIA_ENGINE_RENDER_GL_GLBUFFER_H

#include "sequoia-engine/Core/DoubleBuffered.h"
#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/Buffer.h"
#include "sequoia-engine/Render/GL/GLFwd.h"
#include "sequoia-engine/Render/VertexData.h"
#include <vector>

namespace sequoia {

namespace render {

/// @brief OpenGL abstraction of a hardware buffer
///
/// A `GLBuffer` can contain multiple OpenGL buffers to avoid pipeline stalls of draw commands. If
/// more than one buffer is allocated, the buffers are arranged such that there is a maximum
/// distance between the buffer currently being modfied and the buffer being used for drawing.
///
/// @ingroup gl
class SEQUOIA_API GLBuffer {

  /// OpenGL buffer id
  unsigned int id_;

  /// Target of the buffer(s)
  GLenum target_;

  /// Usage hint of the buffer(s)
  GLenum hint_;

  /// Number of bytes allocated per buffer
  std::size_t numBytes_;

  /// Check if the buffer is locked
  bool isLocked_;

public:
  /// @brief Create the buffer(s)
  ///
  /// @param target       Target of the buffer(s)
  GLBuffer(GLenum target);

  /// @brief Destroy buffer(s)
  ~GLBuffer();

  /// @brief Bind the buffer
  void bind();

  /// @brief Unbind the buffer
  void unbind();

  /// @brief Lock the buffer
  ///
  /// This will bind the modify buffer.
  void* lock(Buffer::LockOption option);

  /// @brief Unlock the buffer
  ///
  /// This will bind the modify buffer.
  void unlock();

  /// @brief Check if the buffer is locked
  bool isLocked() const noexcept { return isLocked_; }

  /// @brief Allocate `numBytes` for the buffer(s)
  ///
  /// This will bind *all* buffers.
  ///
  /// @param numBytes     Number of bytes to allocate
  /// @param hint         Buffer usage hint
  void allocate(std::size_t numBytes, Buffer::UsageHint hint);

  /// @brief Write `length` bytes, starting at `offset`, from `src` to the buffer currently set for
  /// modification
  ///
  /// This will bind the modify buffer.
  ///
  /// @param src            Data pointer of size `numBytes`
  /// @param offset         The *byte* offset from the start of the buffer to start writing
  /// @param length         Number of *bytes* to wrie
  /// @param discardBuffer  If true, will discard the entire content of this buffer before copying
  void write(const void* src, std::size_t offset, std::size_t length, bool discardBuffer = true);

  /// @brief Read `length` bytes, starting from `offset`, from the buffer currently set for
  /// modification and place it in the memory pointed to by `dest`
  ///
  /// This will bind the modify buffer.
  ///
  /// @note `dest` needs to point to a system RAM buffer of sufficient length to accomodate the data
  ///
  /// @param offset   The *byte* offset from the start of the buffer to read
  /// @param length   Number of *bytes* to read
  /// @param dest     Destination to write the data (must be atleast of size `length`)
  void read(std::size_t offset, std::size_t length, void* dest);

  /// @brief Get the OpenGL buffer ID
  unsigned int getID() const;

  /// @brief Convert buffer to string
  std::string toString() const;
};

} // namespace render

} // namespace sequoia

#endif
