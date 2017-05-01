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

#ifndef SEQUOIA_CORE_COMPILER_H
#define SEQUOIA_CORE_COMPILER_H

#include "sequoia/Core/Config.h"

#ifndef __has_feature
#define __has_feature(x) 0
#endif

#ifndef __has_extension
#define __has_extension(x) 0
#endif

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif

#ifndef __has_cpp_attribute
#define __has_cpp_attribute(x) 0
#endif

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#ifndef __has_include
#define __has_include(x) 0
#endif

#if defined(__clang__)
#define SEQUOIA_COMPILER_CLANG 1
#endif

#if defined(__ICC) || defined(__INTEL_COMPILER)
#define SEQUOIA_COMPILER_INTEL 1
#endif

#if defined(__GNUC__) || defined(__GNUG__)
#define SEQUOIA_COMPILER_GNU 1
#endif

#if defined(_MSC_VER)
#define SEQUOIA_COMPILER_MSVC 1
#endif

#if __cplusplus > 201402L
#define SEQUOIA_HAS_CXX17 1
#endif

/// @macro SEQUOIA_GNUC_PREREQ
/// @brief Extend the default `__GNUC_PREREQ` even if glibc's `features.h` isn't available
/// @ingroup core
#ifndef SEQUOIA_GNUC_PREREQ
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
#define SEQUOIA_GNUC_PREREQ(maj, min, patch)                                                       \
  ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) + __GNUC_PATCHLEVEL__ >=                              \
   ((maj) << 20) + ((min) << 10) + (patch))
#elif defined(__GNUC__) && defined(__GNUC_MINOR__)
#define SEQUOIA_GNUC_PREREQ(maj, min, patch)                                                       \
  ((__GNUC__ << 20) + (__GNUC_MINOR__ << 10) >= ((maj) << 20) + ((min) << 10))
#else
#define SEQUOIA_GNUC_PREREQ(maj, min, patch) 0
#endif
#endif

/// @macro SEQUOIA_API_EXPORT
/// @brief Define helpers to create portable import / export macros for each module
#ifdef SEQUOIA_ON_WIN32
#define SEQUOIA_API_EXPORT __declspec(dllexport)
#define SEQUOIA_API_IMPORT __declspec(dllimport)
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#else
#if SEQUOIA_GNUC_PREREQ(4, 0, 0)
#define SEQUOIA_API_EXPORT __attribute__((__visibility__("default")))
#define SEQUOIA_API_IMPORT __attribute__((__visibility__("default")))
#else
#define SEQUOIA_API_EXPORT
#define SEQUOIA_API_IMPORT
#endif
#endif

/// @macro SEQUOIA_BUILTIN_UNREACHABLE
/// @brief Indicate unreachable state
///
/// On compilers which support it, expands to an expression which states that it is undefined
/// behaviour for the compiler to reach this point. Otherwise is not defined.
///
/// @ingroup core
#if __has_builtin(__builtin_unreachable) || SEQUOIA_GNUC_PREREQ(4, 5, 0)
#define SEQUOIA_BUILTIN_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#define SEQUOIA_BUILTIN_UNREACHABLE __assume(false)
#endif

/// @macro SEQUOIA_ATTRIBUTE_ALWAYS_INLINE
/// @brief Mark a method as "always inline" for performance reasons
/// @ingroup core
#if __has_attribute(always_inline) || SEQUOIA_GNUC_PREREQ(4, 0, 0)
#define SEQUOIA_ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define SEQUOIA_ATTRIBUTE_ALWAYS_INLINE __forceinline
#else
#define SEQUOIA_ATTRIBUTE_ALWAYS_INLINE
#endif

/// @macro SEQUOIA_ATTRIBUTE_NORETURN
/// @brief Mark a method as "no return"
/// @ingroup core
#ifdef __GNUC__
#define SEQUOIA_ATTRIBUTE_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define SEQUOIA_ATTRIBUTE_NORETURN __declspec(noreturn)
#else
#define SEQUOIA_ATTRIBUTE_NORETURN
#endif

/// @macro SEQUOIA_BUILTIN_LIKELY
/// @brief Mark this expression as being likely evaluated to "true"
/// @ingroup core
#if __has_builtin(__builtin_expect) || SEQUOIA_GNUC_PREREQ(4, 5, 0)
#define SEQUOIA_BUILTIN_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#define SEQUOIA_BUILTIN_LIKELY(x) (x)
#endif

/// @macro SEQUOIA_BUILTIN_UNLIKELY
/// @brief Mark this expression as being likely evaluated to "false"
/// @ingroup core
#if __has_builtin(__builtin_expect) || SEQUOIA_GNUC_PREREQ(4, 5, 0)
#define SEQUOIA_BUILTIN_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define SEQUOIA_BUILTIN_UNLIKELY(x) (x)
#endif

/// @macro SEQUOIA_ALIGNAS
/// @brief Used to specify a minimum alignment for a structure or variable
/// @ingroup core
#if __GNUC__ && !__has_feature(cxx_alignas) && !SEQUOIA_GNUC_PREREQ(4, 8, 1)
#define SEQUOIA_ALIGNAS(x) __attribute__((aligned(x)))
#else
#define SEQUOIA_ALIGNAS(x) alignas(x)
#endif

/// @macro SEQUOIA_ATTRIBUTE_UNUSED
/// @brief Indicate a function, variable or class is unused
///
/// Some compilers warn about unused functions. When a function is sometimes used or not depending
/// on build settings (e.g. a function only called from  within "assert"), this attribute can be
/// used to suppress such warnings.
///
/// However, it shouldn't be used for unused *variables*, as those have a much more portable
/// solution:
///
/// @code
///   (void)unused_var_name;
/// @endcode
///
/// Prefer cast-to-void wherever it is sufficient.
/// @ingroup core
#if __has_attribute(unused) || SEQUOIA_GNUC_PREREQ(3, 1, 0)
#define SEQUOIA_ATTRIBUTE_UNUSED __attribute__((__unused__))
#else
#define SEQUOIA_ATTRIBUTE_UNUSED
#endif

/// @macro SEQUOIA_NORETURN
/// @brief Indicate a function will never return
/// @ingroup core
#ifdef __GNUC__
#define SEQUOIA_ATTRIBUTE_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define SEQUOIA_ATTRIBUTE_NORETURN __declspec(noreturn)
#else
#define SEQUOIA_ATTRIBUTE_NORETURN
#endif

/// @macro SEQUOIA_CURRENT_FUNCTION
/// @brief Name of the current function
/// @ingroup core
#if defined(_MSC_VER)
#define SEQUOIA_CURRENT_FUNCTION __FUNCSIG__
#elif defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) ||                      \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define SEQUOIA_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define SEQUOIA_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define SEQUOIA_CURRENT_FUNCTION __FUNCSIG__
#elif(defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) ||                                   \
    (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define SEQUOIA_CURRENT_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define SEQUOIA_CURRENT_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define SEQUOIA_CURRENT_FUNCTION __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define SEQUOIA_CURRENT_FUNCTION __func__
#else
#define SEQUOIA_CURRENT_FUNCTION "(unknown)"
#endif

#endif
