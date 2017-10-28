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

#ifndef SEQUOIA_ENGINE_RENDER_VERTEXBUFFER_H
#define SEQUOIA_ENGINE_RENDER_VERTEXBUFFER_H

#include "sequoia-engine/Render/Buffer.h"
#include "sequoia-engine/Render/Vertex.h"

namespace sequoia {

namespace render {

/// @brief Harware buffer for storing vertices
/// @ingroup render
class SEQUOIA_API VertexBuffer : public Buffer {
public:
  VertexBuffer(BufferKind kind, const VertexLayout* layout);

  /// @brief Free all memory
  virtual ~VertexBuffer();

  /// @copydoc Buffer::isSystemRAM
  virtual bool isSystemRAM() const override = 0;

  /// @brief Allocate `numIndices`
  void allocateVertices(std::size_t numVertices, Buffer::UsageHint hint);

  /// @brief Get the number of vertices
  std::size_t getNumVertices() const { return getNumBytes() / layout_->SizeOf; }

  /// @brief Get the layout of the vertices
  const VertexLayout* getLayout() const { return layout_; }

  static bool classof(const Buffer* buffer) {
    return buffer->getKind() >= BK_VertexBuffer && buffer->getKind() < BK_VertexBufferLast;
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
  /// Layout of each individual vertex
  const VertexLayout* layout_;

private:
  using Base = Buffer;
};

} // namespace render

} // namespace sequoia

#endif
