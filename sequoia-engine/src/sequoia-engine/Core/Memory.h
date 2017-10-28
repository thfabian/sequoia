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

#ifndef SEQUOIA_ENGINE_CORE_MEMORY_H
#define SEQUOIA_ENGINE_CORE_MEMORY_H

#include "sequoia-engine/Core/Compiler.h"
#include <boost/align/aligned_alloc.hpp>
#include <boost/align/aligned_allocator.hpp>
#include <boost/align/aligned_delete.hpp>
#include <boost/align/is_aligned.hpp>
#include <memory>
#include <tbb/cache_aligned_allocator.h>

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

/// @brief Memory allocation routines
namespace memory {

/// @brief Default alignment (32 Bytes)
static constexpr std::size_t DefaultAlignment = 32;

/// @brief Generic deleter for `Alloctor`
template <typename Allocator>
struct deleter {
  deleter(const Allocator& a) : a_(a) {}

  typedef typename std::allocator_traits<Allocator>::pointer pointer;

  void operator()(pointer p) const {
    Allocator alloc(a_);
    std::allocator_traits<Allocator>::destroy(alloc, std::addressof(*p));
    std::allocator_traits<Allocator>::deallocate(alloc, p, 1);
  }

  Allocator a_;
};

/// @brief Cache Aligned allocator
template <class T>
using cache_aligned_allocator = tbb::cache_aligned_allocator<T>;

/// @brief Aligned allocator
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

} // nammespace memory

/// @}

} // namespace core

namespace memory {
using namespace core::memory;
}

} // namespace sequoia

#endif
