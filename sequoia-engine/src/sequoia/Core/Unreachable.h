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

#ifndef SEQUOIA_CORE_UNREACHABLE_H
#define SEQUOIA_CORE_UNREACHABLE_H

#include "sequoia/Core/Export.h"

namespace sequoia {

namespace core {

/// @ingroup core
/// @{

/// @fn sequoia_unreachable_internal
/// @brief This function calls abort() and prints the optional message to stderr
///
/// Use the sequoia_unreachable macro (that adds location info), instead of calling this function
/// directly.
SEQUOIA_API SEQUOIA_ATTRIBUTE_NORETURN void
sequoia_unreachable_internal(const char* msg = nullptr, const char* file = nullptr,
                             unsigned line = 0);

/// @macro sequoia_unreachable
/// @brief Marks that the current location is not supposed to be reachable
///
/// In `!NDEBUG` builds, prints the message and location info to stderr. In `NDEBUG` builds, becomes
/// an optimizer hint that the current location is not supposed to be reachable. On compilers that
/// don't support such hints, prints a reduced message instead.
#ifndef NDEBUG
#define sequoia_unreachable(msg)                                                                   \
  sequoia::core::sequoia_unreachable_internal(msg, __FILE__, __LINE__)
#elif defined(SEQUOIA_BUILTIN_UNREACHABLE)
#define sequoia_unreachable(msg) SEQUOIA_BUILTIN_UNREACHABLE
#else
#define sequoia_unreachable(msg) sequoia::core::sequoia_unreachable_internal()
#endif

/// @}

} // namespace core

} // namespace sequoia

#endif
