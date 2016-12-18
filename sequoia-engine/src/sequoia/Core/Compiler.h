/*===-- sequoia/Core/Compiler.h -----------------------------------------------------*- C++ -*-===*\
 *
 *                                      S E Q U O I A
 *
 * This file is distributed under the MIT License (MIT). 
 * See LICENSE.txt for details.
 *
 *===------------------------------------------------------------------------------------------===//
 *
 *! @file
 *! Compiler specific definitions.
 *!
 *! This is a C compliant header file.
 *
\*===------------------------------------------------------------------------------------------===*/

#ifndef SEQUOIA_CORE_COMPILER_H
#define SEQUOIA_CORE_COMPILER_H

/*! @addtogroup core
 *! @{
 */
#include "sequoia/Core/Config.h"
#include <boost/version.hpp>

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

/*! @macro SEQUOIA_GNUC_PREREQ
 *! @brief Extend the default __GNUC_PREREQ even if glibc's features.h isn't available
 */
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

/*! @macro SEQUOIA_BUILTIN_UNREACHABLE
 *! @brief Indicate unreachable state
 *!
 *! On compilers which support it, expands to an expression which states that it is undefined
 *! behaviour for the compiler to reach this point. Otherwise is not defined.
 */
#if __has_builtin(__builtin_unreachable) || SEQUOIA_GNUC_PREREQ(4, 5, 0)
#define SEQUOIA_BUILTIN_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#define SEQUOIA_BUILTIN_UNREACHABLE __assume(false)
#endif

/*! @macro SEQUOIA_ATTRIBUTE_ALWAYS_INLINE
 *! @brief Mark a method as "always inline" for performance reasons
 */
#if __has_attribute(always_inline) || SEQUOIA_GNUC_PREREQ(4, 0, 0)
#define SEQUOIA_ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define SEQUOIA_ATTRIBUTE_ALWAYS_INLINE __forceinline
#else
#define SEQUOIA_ATTRIBUTE_ALWAYS_INLINE
#endif

/*! @macro SEQUOIA_ATTRIBUTE_NORETURN
 *! @brief Mark a method as "no return"
 */
#ifdef __GNUC__
#define SEQUOIA_ATTRIBUTE_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define SEQUOIA_ATTRIBUTE_NORETURN __declspec(noreturn)
#else
#define SEQUOIA_ATTRIBUTE_NORETURN
#endif

/*! @macro SEQUOIA_BUILTIN_LIKELY
 *! @brief Mark this expression as being likely evaluated to "true"
 */
#if __has_builtin(__builtin_expect) || SEQUOIA_GNUC_PREREQ(4, 5, 0)
#define SEQUOIA_BUILTIN_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#define SEQUOIA_BUILTIN_LIKELY(x) (x)
#endif

/*! @macro SEQUOIA_BUILTIN_UNLIKELY
 *! @brief Mark this expression as being likely evaluated to "false"
 */
#if __has_builtin(__builtin_expect) || SEQUOIA_GNUC_PREREQ(4, 5, 0)
#define SEQUOIA_BUILTIN_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define SEQUOIA_BUILTIN_UNLIKELY(x) (x)
#endif

/*! @}
 */

#endif
