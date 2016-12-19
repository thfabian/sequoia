//===-- sequoia/Core/AlignOf.h ------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
// This file defines the AlignedCharArray and AlignedCharArrayUnion classes.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_ALIGNOF_H
#define SEQUOIA_CORE_ALIGNOF_H

#include "sequoia/Core/Core.h"
#include <cstddef>

namespace sequoia {

namespace core {

/// @struct AlignedCharArray
/// @brief Helper for building an aligned character array type
///
/// This template is used to explicitly build up a collection of aligned character array types.
/// We have to build these up using a macro and explicit specialization to cope with MSVC (
/// at least till 2015) where only an  integer literal can be used to specify an alignment
/// constraint. Once built up here, we can then begin to indirect between these using normal C++
/// template parameters.

#ifndef SEQUOIA_COMPILER_MSVC

template <std::size_t Alignment, std::size_t Size>
struct AlignedCharArray {
  SEQUOIA_ALIGNAS(Alignment) char buffer[Size];
};

#else // _MSC_VER

/// @brief Create a type with an aligned char buffer
template <std::size_t Alignment, std::size_t Size>
struct AlignedCharArray;

// We provide special variations of this template for the most common
// alignments because __declspec(align(...)) doesn't actually work when it is
// a member of a by-value function argument in MSVC, even if the alignment
// request is something reasonably like 8-byte or 16-byte. Note that we can't
// even include the declspec with the union that forces the alignment because
// MSVC warns on the existence of the declspec despite the union member forcing
// proper alignment.

template <std::size_t Size>
struct AlignedCharArray<1, Size> {
  union {
    char aligned;
    char buffer[Size];
  };
};

template <std::size_t Size>
struct AlignedCharArray<2, Size> {
  union {
    short aligned;
    char buffer[Size];
  };
};

template <std::size_t Size>
struct AlignedCharArray<4, Size> {
  union {
    int aligned;
    char buffer[Size];
  };
};

template <std::size_t Size>
struct AlignedCharArray<8, Size> {
  union {
    double aligned;
    char buffer[Size];
  };
};

// The rest of these are provided with a __declspec(align(...)) and we simply
// can't pass them by-value as function arguments on MSVC.

#define LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(x)                                                \
  template <std::size_t Size>                                                                      \
  struct AlignedCharArray<x, Size> {                                                               \
    __declspec(align(x)) char buffer[Size];                                                        \
  };

LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(16)
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(32)
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(64)
LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT(128)

#undef LLVM_ALIGNEDCHARARRAY_TEMPLATE_ALIGNMENT

#endif // _MSC_VER

namespace internal {

template <typename T1, typename T2 = char, typename T3 = char, typename T4 = char,
          typename T5 = char, typename T6 = char, typename T7 = char, typename T8 = char,
          typename T9 = char, typename T10 = char>
class AlignerImpl {
  T1 t1;
  T2 t2;
  T3 t3;
  T4 t4;
  T5 t5;
  T6 t6;
  T7 t7;
  T8 t8;
  T9 t9;
  T10 t10;

  AlignerImpl() = delete;
};

template <typename T1, typename T2 = char, typename T3 = char, typename T4 = char,
          typename T5 = char, typename T6 = char, typename T7 = char, typename T8 = char,
          typename T9 = char, typename T10 = char>
union SizerImpl {
  char arr1[sizeof(T1)], arr2[sizeof(T2)], arr3[sizeof(T3)], arr4[sizeof(T4)], arr5[sizeof(T5)],
      arr6[sizeof(T6)], arr7[sizeof(T7)], arr8[sizeof(T8)], arr9[sizeof(T9)], arr10[sizeof(T10)];
};

} // namespace internal

/// @brief This union template exposes a suitably aligned and sized character array member which
/// can hold elements of any of up to ten types.
///
/// These types may be arrays, structs, or any other types. The goal is to expose a char array
/// buffer member which can be used as suitable storage for a placement new of any of these types.
/// Support for more than ten types can be added at the cost of more boilerplate.
///
/// @ingroup core
template <typename T1, typename T2 = char, typename T3 = char, typename T4 = char,
          typename T5 = char, typename T6 = char, typename T7 = char, typename T8 = char,
          typename T9 = char, typename T10 = char>
struct AlignedCharArrayUnion
    : sequoia::core::AlignedCharArray<
          alignof(sequoia::core::internal::AlignerImpl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>),
          sizeof(::sequoia::core::internal::SizerImpl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>)> {};

} // namespace core

} // namespace sequoia

#endif
