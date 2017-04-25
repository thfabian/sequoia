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

#ifndef SEQUOIA_CORE_ASSERT_H
#define SEQUOIA_CORE_ASSERT_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Compiler.h"

#if defined(SEQUOIA_DISABLE_ASSERTS) || defined(NDEBUG)

#define SEQUOIA_ASSERT(expr) ((void)0)
#define SEQUOIA_ASSERT_MSG(expr, msg) ((void)0)

#else

namespace sequoia {

namespace core {

SEQUOIA_CORE_API extern void assertionFailed(const char* expr, const char* function, 
                                                const char* file, long line);
SEQUOIA_CORE_API extern void assertionFailedMsg(const char* expr, const char* msg,
                                                const char* function, const char* file,
                                                long line);
SEQUOIA_CORE_API extern void assertionFailedMsg(char const* expr, const wchar_t* msg,
                                                char const* function, const char* file,
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
