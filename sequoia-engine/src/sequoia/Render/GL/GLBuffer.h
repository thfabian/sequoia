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

#ifndef SEQUOIA_RENDER_GL_GLBUFFER_H
#define SEQUOIA_RENDER_GL_GLBUFFER_H

#include "sequoia/Core/DoubleBuffered.h"
#include "sequoia/Core/Export.h"
#include "sequoia/Render/Buffer.h"
#include "sequoia/Render/GL/GLFwd.h"
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

  /// OpenGL buffer indices
  std::vector<unsigned int> bufferIds_;

  /// Index to the currently active (modifiable) buffer
  unsigned int modifyBufferIdx_;

  /// Target of the buffer(s)
  GLenum target_;

  /// Usage hint of the buffer(s)
  GLenum hint_;

  /// Number of bytes allocated per buffer
  std::size_t numBytes_;

  /// Check if the buffer is locked
  bool isLocked_;

public:
  /// @brief Specify the kind of buffer to bind
  enum BindKind {
    BK_Unknown = 0,
    BK_Draw,  ///< Bind the buffer which is currently being drawn
    BK_Modify ///< Bind the buffer which is currently used to modify the data
  };

  /// @brief Create the buffer(s)
  ///
  /// @param target       Target of the buffer(s)
  /// @param numBuffers   Number of buffers to allocate
  GLBuffer(GLenum target, int numBuffers = 1);

  /// @brief Destroy buffer(s)
  ~GLBuffer();

  /// @brief Bind the buffer
  void bind(BindKind kind);

  /// @brief Unbind the buffer
  void unbind(BindKind kind);

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

  /// @brief Update the buffers to the next timestep
  ///
  /// This effectively shifts all buffers by one to the right, meaning the current modification
  /// buffer is set to be drawn in the next frame.
  void nextTimestep();

  /// @brief Get the index into `getBufferIds()` of the buffer which is being drawn
  unsigned int getDrawBufferIndex() const;

  /// @brief Get the OpenGL id of the buffer which is being drawn
  unsigned int getDrawBufferID() const;

  /// @brief Get the index into `getBufferIds()` of the buffer which is currently being modified
  unsigned int getModifyBufferIndex() const;

  /// @brief Get the OpenGL id of the buffer which is currently being modified
  unsigned int getModifyBufferID() const;

  /// @brief Get the OpenGL buffer ids
  std::vector<unsigned int> getBuffersIds() const { return bufferIds_; }

  /// @brief Convert buffer to string
  std::string toString() const;
};

} // namespace render

} // namespace sequoia

#endif
