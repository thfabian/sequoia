//===--------------------------------------------------------------------------------*- C++ -*-===//
//                         _____                        _
//                        / ____|                      (_)
//                       | (___   ___  __ _ _   _  ___  _  __ _
//                        \___ \ / _ \/ _` | | | |/ _ \| |/ _` |
//                        ____) |  __/ (_| | |_| | (_) | | (_| |
//                       |_____/ \___|\__, |\__,_|\___/|_|\__,_| - Game Engine
//                                       | |
//                                       |_|
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_ALIGNEDADT_H
#define SEQUOIA_CORE_ALIGNEDADT_H

#include "sequoia/Core/Memory.h"
#include <memory>
#include <new>
#include <vector>

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

/// @brief Aligned version of `std::vector`
template <class T>
using aligned_vector = std::vector<T, memory::aligned_allocator<T>>;

/// @brief Aligned version of `std::unique_ptr`
template <class T>
using aligned_unique_ptr = std::unique_ptr<T, memory::aligned_delete>;

/// @brief Aligned version of `std::make_unique`
template <class T, class... Args>
inline aligned_unique_ptr<T> aligned_make_unique(Args&&... args) {
  static_assert(!std::is_array<T>::value, "array version of aligned_make_unique is not supported");

  auto p = memory::aligned_alloc(sizeof(T));
  if(!p)
    throw std::bad_alloc();

  try {
    auto q = ::new(p) T(std::forward<Args>(args)...);
    return aligned_unique_ptr<T>(q);
  } catch(...) {
    memory::aligned_free(p);
    throw;
  }
}

/// @}

} // namespace core

} // namespace sequoia

#endif
