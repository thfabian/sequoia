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

#ifndef SEQUOIA_CORE_DOUBLEBUFFERED_H
#define SEQUOIA_CORE_DOUBLEBUFFERED_H

#include <array>
#include <cstdint>
#include <memory>
#include <type_traits>

namespace sequoia {

namespace core {

namespace internal {

template <class T>
struct IsPointer {
  static bool constexpr value = std::is_pointer<T>::value;
};

template <class U>
struct IsPointer<std::shared_ptr<U>> {
  static bool constexpr value = true;
};

template <class U>
struct IsPointer<std::unique_ptr<U>> {
  static bool constexpr value = true;
};

template <bool IsPointer>
struct AssignTrait {
  template <class T>
  static void assign(T& a, const T& b) {
    a = b;
  }
};

template <>
struct AssignTrait<true> {
  template <class T>
  static void assign(T& a, const T& b) {
    *a = *b;
  }
};

template <class T>
inline void assign(T& a, const T& b) {
  AssignTrait<IsPointer<T>::value>::assign(a, b);
}

} // internal namespace

/// @brief Store two versions of type `T`
///
/// Note that type `T` needs to be copyable. A swap will by default copy the content of the old
/// buffer to the new one (`new = old`). If type `T` is a pointer type (including `std::unqiue_ptr`
/// and `std::shared_ptr`) the *pointee* will be copied instead (`*new = *old`)!
///
/// @code{.cpp}
///   DoubleBuffered<std::string> ab("a", "b");
///
///   // Get the currently active buffer (which is by default the first one)
///   assert(ab.get() == "a");
///
///   // The individual buffers can also be accessed via .first() and .second()
///   assert(ab.first() == "a" && ab.second() == "b");
///
///   // Swapping the buffer sets the currently active buffer to the second one and copies the first
///   // buffer into the second (i.e get() still returns "a")
///   ab.swap();
///   assert(ab.get() == "a");
///   assert(&ab.get() == &ab.second());
///
///   // We can freely modify the first version without affecting the current buffer
///   ab.first() = "aaa";
///   assert(ab.get() == "a");
/// @endcode
///
/// @ingroup core
template <class T>
class DoubleBuffered {
private:
  /// Index to the currently active buffer
  std::int8_t idx_;

  /// Storage of the buffers
  std::array<T, 2> data_;

public:
  /// @brief Default initialize the buffers
  DoubleBuffered() : idx_(0), data_{} {}

  /// @brief Explicitly initialize the buffers
  template <class T1, class T2>
  DoubleBuffered(T1&& t1, T2&& t2) : idx_(0), data_{{std::forward<T1>(t1), std::forward<T2>(t2)}} {}

  DoubleBuffered(const DoubleBuffered&) = default;
  DoubleBuffered(DoubleBuffered&&) = default;
  DoubleBuffered& operator=(const DoubleBuffered&) = default;
  DoubleBuffered& operator=(DoubleBuffered&&) = default;

  /// @brief Directly access the first buffer
  T& first() noexcept { return data_[0]; }
  const T& first() const noexcept { return data_[0]; }

  /// @brief Directly access the second buffer
  T& second() noexcept { return data_[1]; }
  const T& second() const noexcept { return data_[1]; }

  /// @brief Get the currently active buffer
  T& get() noexcept { return data_[idx_]; }
  const T& get() const noexcept { return data_[idx_]; }

  /// @brief Swap the buffers and optionally copy the old buffer to the new one
  ///
  /// If `T` is a pointer type (including `std::unqiue_ptr` and `std::shared_ptr`), the *pointee* 
  /// will be copied.
  ///
  /// @param copy   Copy the old buffer to the new one?
  inline void swap(bool copy = true) {
    std::int8_t oldIdx = idx_;
    idx_ = (idx_ + 1) % 2;
    if(copy)
      internal::assign(data_[idx_], data_[oldIdx]);
  }
};

} // namespace core

template <class T>
using DoubleBuffered = core::DoubleBuffered<T>;

} // namespace sequoia

#endif
