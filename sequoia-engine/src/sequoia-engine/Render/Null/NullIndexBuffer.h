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

#ifndef SEQUOIA_ENGINE_RENDER_NULL_NULLINDEXBUFFER_H
#define SEQUOIA_ENGINE_RENDER_NULL_NULLINDEXBUFFER_H

#include "sequoia-engine/Core/HostBuffer.h"
#include "sequoia-engine/Render/IndexBuffer.h"

namespace sequoia {

namespace render {

/// @brief Null index buffer
///
/// This is simply a HostBuffer.
///
/// @ingroup null
class SEQUOIA_API NullIndexBuffer final : public IndexBuffer {
  std::unique_ptr<core::HostBuffer> buffer_; ///< Mock data

public:
  NullIndexBuffer(IndexBuffer::IndexType type);
  ~NullIndexBuffer();

  static bool classof(const Buffer* buffer) { return buffer->getKind() == BK_NullIndexBuffer; }

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
