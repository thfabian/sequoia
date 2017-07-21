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

#ifndef SEQUOIA_RENDER_INDEXBUFFER_H
#define SEQUOIA_RENDER_INDEXBUFFER_H

#include "sequoia/Render/Buffer.h"

namespace sequoia {

namespace render {

class VertexLayout;

/// @brief Harware buffer for storing vertex indices (elements)
/// @ingroup render
class SEQUOIA_API IndexBuffer : public Buffer {
public:
  using Base = Buffer;

  /// @brief Type of the indices
  enum IndexType {
    IT_UInt32 ///< 32 bit unsigned integer
  };

  IndexBuffer(BufferKind kind, IndexType type);

  /// @brief Free the host memory
  virtual ~IndexBuffer();

  /// @copydoc Buffer::write
  virtual void write(const void* src, std::size_t offset, std::size_t length,
                     bool discardBuffer) override = 0;

  /// @copydoc Buffer::read
  virtual void read(std::size_t offset, std::size_t length, void* dest) override = 0;

  /// @copydoc Buffer::isSystemRAM
  virtual bool isSystemRAM() const override = 0;

  /// @brief Get the number of indices
  std::size_t getNumIndices() const { return getNumBytes() / getSizeOfIndexType(); }

  static bool classof(const Buffer* buffer) {
    return buffer->getKind() >= BK_IndexBuffer && buffer->getKind() < BK_IndexBufferLast;
  }

  /// @brief Get the size of the underlying tpye
  std::size_t getSizeOfIndexType() const noexcept;

  /// @brief Get the underlying type
  IndexType getIndexType() const noexcept { return type_; }

protected:
  /// @copydoc Buffer::lockImpl
  virtual void* lockImpl(LockOption option) override = 0;

  /// @copydoc Buffer::unlockImpl
  virtual void unlockImpl() override = 0;

  /// @copydoc Buffer::allocateImpl
  virtual void allocateImpl(std::size_t numBytes, UsageHint usageHint) override = 0;

  /// @copydoc Buffer::toStringImpl
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  IndexType type_; ///< Type of the indices
};

} // namespace render

} // namespace sequoia

#endif
