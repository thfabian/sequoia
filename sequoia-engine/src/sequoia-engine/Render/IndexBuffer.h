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

#include "sequoia-engine/Render/Buffer.h"

namespace sequoia {

namespace render {

/// @brief Harware buffer for storing vertex indices (elements)
/// @ingroup render
class SEQUOIA_API IndexBuffer : public Buffer {
public:
  /// @brief Type of the indices
  enum IndexType {
    IT_UInt8,  ///< 8 bit unsigned integer
    IT_UInt16, ///< 16 bit unsigned integer
    IT_UInt32  ///< 32 bit unsigned integer
  };

  IndexBuffer(BufferKind kind, IndexType type);

  /// @brief Free all memory
  virtual ~IndexBuffer();

  /// @copydoc Buffer::isSystemRAM
  virtual bool isSystemRAM() const override = 0;

  /// @brief Allocate `numIndices`
  void allocateIndices(std::size_t numIndices);

  /// @brief Get the number of indices
  std::size_t getNumIndices() const { return getNumBytes() / getSizeOfIndexType(); }

  /// @brief Get the size of the underlying tpye
  std::size_t getSizeOfIndexType() const noexcept;

  /// @brief Get the underlying type
  IndexType getIndexType() const noexcept { return type_; }

  static bool classof(const Buffer* buffer) {
    return buffer->getKind() >= BK_IndexBuffer && buffer->getKind() < BK_IndexBufferLast;
  }

protected:
  /// @copydoc Buffer::writeImpl
  virtual void writeImpl(const void* src, std::size_t offset, std::size_t length,
                         bool discardBuffer) override = 0;

  /// @copydoc Buffer::readImpl
  virtual void readImpl(std::size_t offset, std::size_t length, void* dest) override = 0;

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

private:
  using Base = Buffer;
};

} // namespace render

} // namespace sequoia

#endif
