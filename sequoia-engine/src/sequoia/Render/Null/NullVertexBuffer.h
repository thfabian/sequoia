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

#ifndef SEQUOIA_RENDER_NULL_NULLVERTEXBUFFER_H
#define SEQUOIA_RENDER_NULL_NULLVERTEXBUFFER_H

#include "sequoia/Render/HostBuffer.h"
#include "sequoia/Render/VertexBuffer.h"

namespace sequoia {

namespace render {

/// @brief Null buffer for storing vertices
///
/// This is simply a HostBuffer.
///
/// @ingroup gl
class SEQUOIA_API NullVertexBuffer final : public VertexBuffer {
  std::unique_ptr<HostBuffer> buffer_; ///< Mock data

public:
  NullVertexBuffer(const VertexLayout* layout, int numBuffers);

  /// @brief Free all memory
  ~NullVertexBuffer();

  /// @copydoc Buffer::isSystemRAM
  bool isSystemRAM() const override { return buffer_->isSystemRAM(); }

  static bool classof(const Buffer* buffer) { return buffer->getKind() == BK_NullVertexBuffer; }

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
  using Base = VertexBuffer;
};

} // namespace render

} // namespace sequoia

#endif
