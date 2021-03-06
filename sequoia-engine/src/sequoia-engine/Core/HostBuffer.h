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

#ifndef SEQUOIA_ENGINE_CORE_HOSTBUFFER_H
#define SEQUOIA_ENGINE_CORE_HOSTBUFFER_H

#include "sequoia-engine/Core/Buffer.h"

namespace sequoia {

namespace core {

/// @brief Buffer allocate in the CPU RAM
/// @ingroup render
class SEQUOIA_API HostBuffer : public Buffer {
  void* dataPtr_; ///< Pointer to host memory

public:
  using Base = Buffer;

  /// @brief Initialize empty buffer
  HostBuffer();

  /// @brief Free the host memory
  virtual ~HostBuffer();

  /// @brief Create a buffer of `numBytes` size
  static std::unique_ptr<HostBuffer> create(std::size_t numBytes);

  /// @brief Get the data pointer
  void* getData() { return dataPtr_; }
  const void* getData() const { return dataPtr_; }

  /// @brief Get the data interpreted as type `T`
  template <class T>
  T* getDataAs() {
    return static_cast<T*>(dataPtr_);
  }
  template <class T>
  const T* getDataAs() const {
    return static_cast<const T*>(dataPtr_);
  }

  static bool classof(const Buffer* buffer) {
    return buffer->getKind() >= BK_HostBuffer && buffer->getKind() < BK_HostBufferLast;
  }

protected:
  /// @copydoc Buffer::writeImpl
  virtual void writeImpl(const void* src, std::size_t offset, std::size_t length,
                         bool discardBuffer) override;

  /// @copydoc Buffer::readImpl
  virtual void readImpl(std::size_t offset, std::size_t length, void* dest) override;

  /// @copydoc Buffer::lockImpl
  virtual void* lockImpl(LockOption option) override {
    (void)option;
    return dataPtr_;
  }

  /// @copydoc Buffer::unlockImpl
  virtual void unlockImpl() override {}

  /// @copydoc Allocate `numBytes` memory, all hints are ignored
  virtual void allocateImpl(std::size_t numBytes, UsageHint usageHint) override;

  /// @copydoc Buffer::toStringImpl
  virtual std::pair<std::string, std::string> toStringImpl() const override;

private:
  /// @brief Free allocated memory
  void free();
};

} // namespace core

} // namespace sequoia

#endif
