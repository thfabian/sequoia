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

#ifndef SEQUOIA_CORE_MUTEX_H
#define SEQUOIA_CORE_MUTEX_H

#include "sequoia/Core/Compiler.h"
#include <mutex>
#include <tbb/mutex.h>
#include <tbb/reader_writer_lock.h>
#include <tbb/spin_mutex.h>

namespace sequoia {

namespace core {

/// @brief Class that models Mutex Concept using a spin lock
///
/// This should be the default lock as it is very lightweight (1 Byte) and extremely fast under low
/// contention.
///
/// @ingroup core
using SpinMutex = tbb::spin_mutex;

/// @brief Writer-preference reader-writer lock with local-only spinning on readers
///
/// Loosely adapted from Mellor-Crummey and Scott pseudocode at
/// http://www.cs.rochester.edu/research/synchronization/pseudocode/rw.html#s_wp
///
/// @ingroup core
using ReadWriteMutex = tbb::reader_writer_lock;

/// @brief Class that models Mutex Concept using underlying OS locks
///
/// Releases the lock if an exception is thrown from the protected region of code.
///
/// @ingroup core
using Mutex = tbb::mutex;

/// @brief Use RAII construct to lock/unlock the `mutex`
/// @ingroup core
#define SEQUOIA_LOCK_GUARD(mutex)                                                                  \
  std::lock_guard<std::decay<decltype(mutex)>::type> sequoia_lock_guard_##__LINE__(mutex)

} // namespace core

using Mutex = core::Mutex;
using SpinMutex = core::SpinMutex;
using ReadWriteMutex = core::ReadWriteMutex;

} // namespace sequoia

#endif
