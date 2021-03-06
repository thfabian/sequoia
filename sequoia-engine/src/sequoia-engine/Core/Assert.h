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

#ifndef SEQUOIA_ENGINE_CORE_ASSERT_H
#define SEQUOIA_ENGINE_CORE_ASSERT_H

#include "sequoia-engine/Core/Compiler.h"
#include "sequoia-engine/Core/Export.h"

namespace sequoia {

namespace core {

SEQUOIA_API extern void assertionFailed(const char* expr, const char* function, const char* file,
                                        long line);
SEQUOIA_API extern void assertionFailedMsg(const char* expr, const char* msg, const char* function,
                                           const char* file, long line);
SEQUOIA_API extern void assertionFailedMsg(char const* expr, wchar_t* msg, char const* function,
                                           char const* file, long line);

} // namespace core

} // namespace sequoia

#ifdef SEQUOIA_DISABLE_ASSERTS

#define SEQUOIA_ENABLE_ASSERTS 0

#define SEQUOIA_ASSERT(expr) ((void)0)
#define SEQUOIA_ASSERT_MSG(expr, msg) ((void)0)

#else

#define SEQUOIA_ENABLE_ASSERTS 1

/// @brief Evaluate assertion
///
/// If the argument expression `expr` of this macro with functional form compares equal to zero
/// (i.e., the expression is false), a message is written to the standard error device and abort is
/// called, terminating the program execution.
///
/// If either `NDEBUG` or `SEQUOIA_DISABLE_ASSERTS` is defined this macro is disabled.
///
/// @ingroup core
#define SEQUOIA_ASSERT(expr)                                                                       \
  (SEQUOIA_BUILTIN_LIKELY(!!(expr))                                                                \
       ? ((void)0)                                                                                 \
       : sequoia::core::assertionFailed(#expr, SEQUOIA_CURRENT_FUNCTION, __FILE__, __LINE__))

/// @brief Evaluate assertion and print extra error message
///
/// Like @ref SEQUOIA_ASSERT but print an extra error message `msg` on failure.
///
/// @ingroup core
#define SEQUOIA_ASSERT_MSG(expr, msg)                                                              \
  (SEQUOIA_BUILTIN_LIKELY(!!(expr)) ? ((void)0) : sequoia::core::assertionFailedMsg(               \
                                                      #expr, msg, SEQUOIA_CURRENT_FUNCTION,        \
                                                      __FILE__, __LINE__))

#endif

#endif
