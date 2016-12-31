//===-- sequoia/Core/Assert.h -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_ASSERT_H
#define SEQUOIA_CORE_ASSERT_H

#include "sequoia/Core/Core.h"

#if defined(SEQUOIA_DISABLE_ASSERTS) || defined(NDEBUG)

#define SEQUOIA_ASSERT(expr) ((void)0)
#define SEQUOIA_ASSERT_MSG(expr, msg) ((void)0)

#else

namespace sequoia {

namespace core {

SEQUOIA_CORE_EXPORT extern void assertionFailed(char const* expr, char const* function,
                                                char const* file, long line);
SEQUOIA_CORE_EXPORT extern void assertionFailedMsg(char const* expr, char const* msg,
                                                   char const* function, char const* file,
                                                   long line);
} // namespace core

} // namespace sequoia

#define SEQUOIA_ASSERT(expr)                                                                       \
  (SEQUOIA_BUILTIN_LIKELY(!!(expr))                                                                \
       ? ((void)0)                                                                                 \
       : sequoia::core::assertionFailed(#expr, SEQUOIA_CURRENT_FUNCTION, __FILE__, __LINE__))
#define SEQUOIA_ASSERT_MSG(expr, msg)                                                              \
  (SEQUOIA_BUILTIN_LIKELY(!!(expr)) ? ((void)0) : sequoia::core::assertionFailedMsg(               \
                                                      #expr, msg, SEQUOIA_CURRENT_FUNCTION,        \
                                                      __FILE__, __LINE__))

#endif

#endif