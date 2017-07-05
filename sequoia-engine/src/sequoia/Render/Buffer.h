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
#include "sequoia/Render/RenderSystemObject.h"
#include <memory>
#include <string>

namespace sequoia {

namespace render {

/// @brief Generic hardware buffer
/// @ingroup render
class SEQUOIA_API Buffer : public RenderSystemObject,
                           public NonCopyable,
                           public std::enable_shared_from_this<Buffer> {
private:
  void* data_; ///< Currently locked data pointer

public:
  /// @brief Usage hint of buffer
  enum BufferUsageKind {
    BK_Invalid = 0,

    /// Static buffer which the application rarely modifies once created. Modifying the contents of
    /// this buffer will involve a performance hit.
    BK_Static = 1,

    /// Indicates the application would like to modify this buffer with the CPU fairly often.
    /// This is the least optimal buffer setting.
    BK_Dynamic = 2,

    /// Indicates the application will never read the contents of the buffer back,
    /// it will only ever write data.
    BK_WriteOnly = 4,

    /// Indicates that the application will be refilling the contents of the buffer regularly (not
    /// just updating, but generating the contents from scratch), and therefore does not mind if
    /// the contents of the buffer are lost somehow and need to be recreated. This allows and
    /// additional level of optimisation on the buffer.
    BK_Discardable = 8,

    /// Combination of `BK_Static` and `BK_WriteOnly`. This is the optimal buffer usage setting.
    BK_StaticWriteOnly = 5,

    /// Combination of `BK_Dynamic` and `BK_WriteOnly`.
    BK_DynamicWriteOnly = 6,

    /// Combination of `BK_Dynamic`, `BK_WriteOnly` and `BK_Discardable`. This means that you
    /// expect to replace the entire contents of the buffer on an extremely regular basis, most
    /// likely every frame. By selecting this option, you free the system up from having to be
    /// concerned about losing the existing contents of the buffer at any time, because if it does
    /// lose them, you will be replacing them next frame anyway.
    BK_DynamicWriteOnlyDiscardable = 14
  };

  /// @brief
  Buffer(RenderSystemKind kind);

  /// @brief Virutal destructor
  virtual ~Buffer();

  /// @brief Lock the buffer
  void lock() noexcept {
    SEQUOIA_ASSERT_MSG(data_, "buffer already locked");
    data_ = lockImpl();
  };

  /// @brief Unlock the buffer again
  void unlock() noexcept {
    unlockImpl();
    data_ = nullptr;
  }

  /// @brief Get the currently locked data pointer
  ///
  /// @see BufferGuard
  void* get() noexcept {
    SEQUOIA_ASSERT_MSG(data_, "data is NULL, did you forget to lock the buffer?");
    return data_;
  }

  /// @brief Get the number of bytes in the buffer i.e the size
  virtual std::size_t getNumBytes() noexcept = 0;

  /// @brief Convert to string
  virtual std::string toString() const = 0;

protected:
  /// @brief Lock the buffer and return the data pointer to the locked region
  virtual void* lockImpl() noexcept = 0;

  /// @brief Unlock the buffer
  virtual void unlockImpl() noexcept = 0;
};

/// @brief Locks the `buffer` on construction and unlocks it again on destruction
///
/// Instead of using
///
/// @code{.cpp}
///   buffer.lock();
///   void* data = buffer.get();
///   buffer.unlock();
/// @endcode
///
/// you may use
///
/// @code{.cpp}
///   BufferGuard lock(buffer);
///   void* data = buffer.get();
///   // ... destructor invokes unlock()
/// @endcode
///
/// which will assert `unlock` is called.
///
/// @ingroup render
class BufferGuard : public NonCopyable {
  Buffer* buffer_;

public:
  /// @brief Lock buffer
  BufferGuard(Buffer& buffer) : BufferGuard(&buffer) {}
  BufferGuard(std::shared_ptr<Buffer>& buffer) : BufferGuard(buffer.get()) {}
  BufferGuard(std::unique_ptr<Buffer>& buffer) : BufferGuard(buffer.get()) {}
  BufferGuard(Buffer* buffer) : buffer_(buffer) { buffer_->lock(); }

  /// @brief Unlock the buffer
  ~BufferGuard() { buffer_->unlock(); }
};

} // namespace render

} // namespace sequoia

#endif
