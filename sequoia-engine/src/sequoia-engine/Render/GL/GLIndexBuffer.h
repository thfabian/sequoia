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

#ifndef SEQUOIA_ENGINE_RENDER_GL_GLINDEXBUFFER_H
#define SEQUOIA_ENGINE_RENDER_GL_GLINDEXBUFFER_H

#include "sequoia-engine/Render/GL/GLBuffer.h"
#include "sequoia-engine/Render/GL/GLFwd.h"
#include "sequoia-engine/Render/IndexBuffer.h"

namespace sequoia {

namespace render {

/// @brief OpenGL buffer for storing vertex indices (elements)
/// @ingroup gl
class SEQUOIA_API GLIndexBuffer final : public IndexBuffer {
  GLBuffer glBuffer_; ///< OpenGL buffer

public:
  GLIndexBuffer(IndexBuffer::IndexType type);

  /// @brief Free all memory
  ~GLIndexBuffer();

  /// @copydoc Buffer::isSystemRAM
  bool isSystemRAM() const override { return false; }

  /// @brief Bind the buffer for drawing
  void bindForDrawing();

  /// @brief Bind the buffer for modify
  void bindForModify();

  /// @brief Update the buffer to the next timestep
  void nextTimestep();

  /// @brief Get the `GLenum` of the index type
  GLenum getGLIndexType() const;

  static bool classof(const Buffer* buffer) { return buffer->getKind() == BK_GLIndexBuffer; }

protected:
  /// @copydoc Buffer::writeImpl
  virtual void writeImpl(const void* src, std::size_t offset, std::size_t length,
                         bool discardBuffer) override;

  /// @copydoc Buffer::readImpl
  virtual void readImpl(std::size_t offset, std::size_t length, void* dest) override;

  /// @copydoc Buffer::lockImpl
  void* lockImpl(LockOption option) override;

  /// @copydoc Buffer::unlockImpl
  void unlockImpl() override;

  /// @copydoc Buffer::allocateImpl
  void allocateImpl(std::size_t numBytes, UsageHint usageHint) override;

  /// @copydoc Buffer::toStringImpl
  std::pair<std::string, std::string> toStringImpl() const override;

private:
  using Base = IndexBuffer;
};

} // namespace render

} // namespace sequoia

#endif
