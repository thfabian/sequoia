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

#ifndef SEQUOIA_CORE_MEMORY_H
#define SEQUOIA_CORE_MEMORY_H

#include "sequoia/Core/Compiler.h"
#include <boost/align/aligned_alloc.hpp>
#include <boost/align/aligned_allocator.hpp>
#include <boost/align/aligned_delete.hpp>
#include <boost/align/is_aligned.hpp>

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

/// @brief Memory allocation routines
namespace memory {

/// @brief Default alignment (32 Bytes)
static constexpr std::size_t DefaultAlignment = 32;

/// @brief Alligned allocator
template <class T, std::size_t Alignment = DefaultAlignment>
using aligned_allocator = boost::alignment::aligned_allocator<T, Alignment>;

/// @brief This function is used to compare the alignment of a pointer
inline bool is_aligned(const void* ptr, std::size_t alignment = DefaultAlignment) noexcept {
  return boost::alignment::is_aligned(alignment, ptr);
}

/// @brief Allocate space with the specified `size` and `Alignment`
///
/// `aligned_free` shall be used to free the memory again.
inline void* aligned_alloc(std::size_t size, std::size_t alignment = DefaultAlignment) noexcept {
  return boost::alignment::aligned_alloc(alignment, size);
}

/// @brief Free memory allocated with `aligned_alloc`
inline void aligned_free(void* ptr) noexcept { boost::alignment::aligned_free(ptr); }

/// @brief Deleter that destroys the object and then deallocates space using `aligned_free`
class aligned_delete {
public:
  template <class T>
  void operator()(T* ptr) const noexcept(noexcept(ptr->~T())) {
    if(ptr) {
      ptr->~T();
      aligned_free(ptr);
    }
  }
};

} // nammespace alignment

/// @}

} // namespace core

namespace memory {
using namespace core::memory;
}

} // namespace sequoia

#endif
