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

#ifndef SEQUOIA_CORE_PRETTYSTACKTRACE_H
#define SEQUOIA_CORE_PRETTYSTACKTRACE_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Singleton.h"
#include <memory>

namespace sequoia {

namespace core {

/// @brief PrettyStackTrace is used to make crashes give more contextual information about what the
/// program was doing when it crashed
///
/// The stack trace may be incomplete if the program was compiled without debug symbols. To disable
/// the stack trace mechanism define `NDEBUG` or `SEQUOIA_DISABLE_ASSERTS`.
///
/// @note
/// Stack traces are only supported on Linux, it will compile fine under other platforms but will
/// not do anything.
///
/// @ingroup core
class SEQUOIA_CORE_API PrettyStackTrace : public Singleton<PrettyStackTrace> {
  class Impl;
  Impl* impl_;

public:
  PrettyStackTrace();
  ~PrettyStackTrace();
};

} // namespace core

} // namespace sequoia

#endif
