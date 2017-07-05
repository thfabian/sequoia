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

#ifndef SEQUOIA_RENDER_BUFFER_H
#define SEQUOIA_RENDER_BUFFER_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Export.h"
#include "sequoia/Core/NonCopyable.h"
#include <memory>
#include <string>

namespace sequoia {

namespace render {

/// @brief Generic hardware buffer which can hold any area of memory: system memory RAM, Video RAM
/// on the GPUetc.
///
/// Buffers have the ability to be 'shadowed' in system memory (see HostBuffer) this is because the
/// kinds of access allowed on hardware buffers which are store in video RAM is not always as
/// flexible as that allowed for areas of system  memory.
/// For example, it is often either impossible, or extremely undesirable from a performance
/// standpoint to read from a hardware buffer; when writing to hardware buffers, you should also
/// write every byte and do it sequentially. In situations where this is too restrictive,
/// it is possible to create a hardware, write-only buffer (the most efficient kind)
/// and back it with a system memory 'shadow' copy which can be read and updated arbitrarily.
///
/// @ingroup render
class SEQUOIA_API Buffer : public NonCopyable {
public:
  /// @brief RTTI discriminator
  enum BufferKind {
    BK_HostBuffer,
    BK_IndexBuffer,
    BK_GLIndexBuffer,
    BK_IndexBufferLast,
    BK_VertexBuffer,
    BK_GLVertexBuffer,
    BK_VertexBufferLast,
  };

  /// @brief Usage hint of buffer
  enum UsageHint {
    UH_Invalid = 0,

    /// Static buffer which the application rarely modifies once created. Modifying the contents of
    /// this buffer will involve a performance hit.
    UH_Static = 1,

    /// Indicates the application would like to modify this buffer with the CPU fairly often.
    /// This is the least optimal buffer setting.
    UH_Dynamic = 2,

    /// Indicates the application will never read the contents of the buffer back,
    /// it will only ever write data.
    UH_WriteOnly = 4,

    /// Indicates that the application will be refilling the contents of the buffer regularly (not
    /// just updating, but generating the contents from scratch), and therefore does not mind if
    /// the contents of the buffer are lost somehow and need to be recreated. This allows and
    /// additional level of optimisation on the buffer.
    UH_Discardable = 8,

    /// Combination of `UH_Static` and `UH_WriteOnly`. This is the optimal buffer usage setting.
    UH_StaticWriteOnly = 5,

    /// Combination of `UH_Dynamic` and `UH_WriteOnly`.
    UH_DynamicWriteOnly = 6,

    /// Combination of `UH_Dynamic`, `UH_WriteOnly` and `UH_Discardable`. This means that you
    /// expect to replace the entire contents of the buffer on an extremely regular basis, most
    /// likely every frame. By selecting this option, you free the system up from having to be
    /// concerned about losing the existing contents of the buffer at any time, because if it does
    /// lose them, you will be replacing them next frame anyway.
    UH_DynamicWriteOnlyDiscardable = 14
  };

  /// @brief Specifiy the kind of locking
  enum LockOptionKind {
    /// Normal mode, ie allows read/write and contents are preserved. This kind of lock allows
    /// reading and writing from the buffer -- it’s also the least optimal because basically you’re
    /// telling the card you could be doing anything at all. If you’re not using a shadow buffer,
    /// it requires the buffer to be transferred from the card and back again.
    /// If you’re using a shadow buffer the effect is minimal.
    LK_Normal,

    /// Discards the **entire** buffer while locking, this usually yields the highest performance.
    /// This means you are happy for the card to discard the entire current contents of the
    /// buffer. Implicitly this means you are not going to read the data - it also means
    /// that the card can avoid any stalls if the buffer is currently being rendered from, because
    /// it will actually give you an entirely different one.
    LK_Discard,

    /// Lock the buffer for reading only. Not allowed in buffers which are created with
    /// `UH_WriteOnly`
    LK_ReadOnly,

    /// Lock the buffer for writing only.
    LK_WriteOnly
  };

  Buffer(BufferKind kind);

  /// @brief Virutal destructor
  virtual ~Buffer();

  /// @brief Allocate `numBytes`
  void allocate(std::size_t numBytes, UsageHint usageHint) {
    numBytes_ = numBytes;
    usageHint_ = usageHint;
    allocateImpl(numBytes, usageHint);
  }

  /// @brief Lock the buffer
  ///
  /// Note that locking in this context does **not** refer to mutual exclusion of the buffer object!
  /// It merely indicates to the buffer that it's being accessed.
  ///
  /// @param lockType   Strategy used to lock the buffer.
  void lock(LockOptionKind option) {
    SEQUOIA_ASSERT_MSG(!isLocked(), "buffer already locked");

    if(hasShadowBuffer()) {
      // Lock the shadow buffer and flag it for update
      if(option != LK_ReadOnly)
        shadowBufferIsDirty_ = true;

      shadowBuffer_->lock(option);
      data_ = shadowBuffer_->get();
    } else {
      // Lock the real buffer if there is no shadow buffer
      data_ = lockImpl(option);
    }
  };

  /// @brief Unlock the buffer
  void unlock() {
    SEQUOIA_ASSERT_MSG(isLocked(), "buffer not locked");

    if(hasShadowBuffer() && shadowBuffer_->isLocked()) {
      // Potentially update the real buffer from the shadow buffer
      shadowBuffer_->unlock();
      updateFromShadow();
    } else {
      unlockImpl();
    }
    data_ = nullptr;
  }

  /// @brief Check if the buffer is locked
  bool isLocked() const noexcept { return data_ != nullptr; }

  /// @brief Get the currently locked data pointer
  /// @see BufferGuard
  void* get() noexcept {
    SEQUOIA_ASSERT_MSG(data_, "data is NULL, did you forget to lock the buffer?");
    return data_;
  }

  /// @brief Write `length` bytes, starting at `offset`, from `src` to the buffer
  ///
  /// The buffer needs to be of sufficient length to accomodate the data.
  ///
  /// @param src            Data pointer of size `numBytes`
  /// @param offset         The *byte* offset from the start of the buffer to start writing
  /// @param length         Number of *bytes* to wrie
  /// @param discardBuffer  If true, will discard the entire content of this buffer before copying
  virtual void write(const void* src, std::size_t offset, std::size_t length,
                     bool discardBuffer = true) = 0;

  /// @brief Read `length` bytes, starting from `offset`, from the buffer and place it in the
  /// memory pointed to by `dest`
  ///
  /// @param offset   The *byte* offset from the start of the buffer to read
  /// @param length   Number of *bytes* to read
  /// @param dest     Destination to write the data (must be atleast of size `length`)
  virtual void read(std::size_t offset, std::size_t length, void* dest) = 0;

  /// @brief Check if the buffer is allocated in system RAM
  virtual bool isSystemRAM() const = 0;

  /// @brief Set the shadow buffer
  ///
  /// This takes ownership of the `buffer`
  void setShadowBuffer(std::unique_ptr<Buffer>&& buffer) noexcept {
    shadowBuffer_ = std::move(buffer);
  }

  /// @brief Check if the buffer has a shadow buffer in the core system RAM
  bool hasShadowBuffer() const noexcept { return shadowBuffer_ != nullptr; };

  /// @brief Get the usage hint of the buffer
  UsageHint getUsageHint() const noexcept { return usageHint_; }

  /// @brief Get the number of bytes in the buffer i.e the size
  std::size_t getNumBytes() const noexcept { return numBytes_; }

  /// @brief Convert to string
  std::string toString() const;

  /// @brief Get the kind of buffer
  BufferKind getKind() const noexcept { return kind_; }

protected:
  /// @brief Lock the buffer and return the data pointer to the locked region
  virtual void* lockImpl(LockOptionKind option) = 0;

  /// @brief Unlock the buffer
  virtual void unlockImpl() = 0;

  /// @brief Allocate `numBytes`
  virtual void allocateImpl(std::size_t numBytes, UsageHint usageHint) = 0;

  /// @brief Implementation of `toString` returns stringified members and title
  virtual std::pair<std::string, std::string> toStringImpl() const;

  /// @brief Update the real buffer from the shadow buffer (if necessary)
  void updateFromShadow();

  /// @brief Set the number of bytes
  void setNumBytes(std::size_t numBytes) { numBytes_ = numBytes; }

private:
  /// Currently locked data pointer. This may point to a region in system RAM or device
  mutable void* data_;

  /// Number of allocated bytes
  std::size_t numBytes_;

  /// Usage hint of the buffer
  UsageHint usageHint_;

  /// Indicate if the shadow buffer needs updating
  bool shadowBufferIsDirty_;

  /// Shadow buffer (if any)
  std::unique_ptr<Buffer> shadowBuffer_;

  /// Type of buffer (for RTTI)
  BufferKind kind_;
};

/// @brief Locks the `buffer` on construction and unlocks it again on destruction
///
/// Instead of using
///
/// @code{.cpp}
///   buffer.lock(Buffer::LK_Discard);
///   void* data = buffer.get();
///   buffer.unlock();
/// @endcode
///
/// you may use
///
/// @code{.cpp}
///   BufferGuard lock(buffer, Buffer::LK_Discard);
///   void* data = buffer.get();
///   // ... destructor invokes unlock()
/// @endcode
///
/// which will assert `unlock` is called (even in case of an exception).
///
/// @ingroup render
class BufferGuard : public NonCopyable {
  Buffer* buffer_; ///< Reference to the buffer

public:
  /// @brief Lock buffer
  BufferGuard(Buffer& buffer, Buffer::LockOptionKind option) : BufferGuard(&buffer, option) {}
  BufferGuard(std::shared_ptr<Buffer>& buffer, Buffer::LockOptionKind option)
      : BufferGuard(buffer.get(), option) {}
  BufferGuard(std::unique_ptr<Buffer>& buffer, Buffer::LockOptionKind option)
      : BufferGuard(buffer.get(), option) {}
  BufferGuard(Buffer* buffer, Buffer::LockOptionKind option) : buffer_(buffer) {
    buffer_->lock(option);
  }

  /// @brief Unlock the buffer
  ~BufferGuard() { buffer_->unlock(); }
};

} // namespace render

} // namespace sequoia

#endif
