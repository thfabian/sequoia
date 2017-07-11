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
#include <condition_variable>
#include <mutex>
#include <tbb/mutex.h>
#include <tbb/spin_mutex.h>
#include <type_traits>

#ifdef SEQUOIA_DEBUG_THREADS
#include "sequoia/Core/Logging.h"
#endif

namespace sequoia {

namespace core {

/// @brief Class that models Mutex Concept using a spin lock
///
/// This should be the default lock as it is very lightweight (1 Byte) and  extremely fast under low
/// contention.
///
/// @ingroup core
using SpinMutex = tbb::spin_mutex;

/// @brief Class that models Mutex Concept using underlying OS locks
///
/// @note Releases the lock if an exception is thrown from the protected region of code.
/// @ingroup core
using Mutex = tbb::mutex;

/// @brief Acquire the given `mutex`
/// @ingroup core
#define SEQUOIA_LOCK(mutex) SEQUOIA_LOCK_IMPL(mutex, __FILE__, __LINE__)

#ifdef SEQUOIA_DEBUG_THREADS
#define SEQUOIA_LOCK_IMPL(mutex, file, line)                                                       \
  do {                                                                                             \
    LOG(DEBUG) << "LOCK: Acquiring lock: " << file << ":" << line << " ...";                       \
    mutex.lock();                                                                                  \
    LOG(DEBUG) << "LOCK: Successfully acquired lock: " << file << ":" << line;                     \
  } while(0)
#else
#define SEQUOIA_LOCK_IMPL(mutex, file, line) mutex.lock()
#endif

/// @brief Tries to lock the associated `mutex`, returns if `true` if the mutex was acquired,
/// `false` otherwise
/// @ingroup core
#define SEQUOIA_TRY_LOCK(mutex) SEQUOIA_TRY_LOCK_IMPL(mutex, __FILE__, __LINE__)

#ifdef SEQUOIA_DEBUG_THREADS
namespace internal {

template <typename MutexType>
SEQUOIA_INLINE bool tryLock(MutexType& mutex, const char* file, int line) noexcept {
  LOG(DEBUG) << "TRY_LOCK: Trying to acquire lock: " << file << ":" << line << " ...";
  bool res = mutex.try_lock();
  LOG(DEBUG) << "TRY_LOCK: " << (res ? "Successfully acquired" : "Failed to acquire")
             << " lock: " << file << ":" << line;
  return res;
}

} // namespace internal

#define SEQUOIA_TRY_LOCK_IMPL(mutex, file, line) sequoia::core::internal::tryLock(mutex, file, line)
#else
#define SEQUOIA_TRY_LOCK_IMPL(mutex, file, line) mutex.try_lock()
#endif

/// @brief Release the given `mutex`
#define SEQUOIA_UNLOCK(mutex) SEQUOIA_UNLOCK_IMPL(mutex, __FILE__, __LINE__)

#ifdef SEQUOIA_DEBUG_THREADS
#define SEQUOIA_UNLOCK_IMPL(mutex, file, line)                                                     \
  do {                                                                                             \
    LOG(DEBUG) << "UNLOCK: Releasing lock: " << file << ":" << line << " ...";                     \
    mutex.unlock();                                                                                \
    LOG(DEBUG) << "UNLOCK: Successfully released lock: " << file << ":" << line;                   \
  } while(0)
#else
#define SEQUOIA_UNLOCK_IMPL(mutex, file, line) mutex.unlock()
#endif

/// @brief The class LockGuard is a mutex wrapper that provides a convenient RAII-style mechanism
/// for owning a mutex for the duration of a scoped block
///
/// When a LockGuard object is created, it attempts to take ownership of the mutex it is given.
/// When control leaves the scope in which the LockGuard object was created, the LockGuard is
/// destructed and the mutex is released.
///
/// The LockGuard class is non-copyable.
/// @ingroup core
#ifdef SEQUOIA_DEBUG_THREADS
template <typename MutexType>
class LockGuard {
  MutexType& mutex_;

  const char* file_;
  int line_;

public:
  explicit LockGuard(MutexType& mutex, const char* file, int line)
      : mutex_(mutex), file_(file), line_(line) {
    SEQUOIA_LOCK_IMPL(mutex_, file_, line_);
  }

  ~LockGuard() { SEQUOIA_UNLOCK_IMPL(mutex_, file_, line_); }

  LockGuard(const LockGuard&) = delete;
  LockGuard& operator=(const LockGuard&) = delete;
};

#else
template <typename MutexType>
class LockGuard {
  MutexType& mutex_;

public:
  explicit LockGuard(MutexType& mutex) : mutex_(mutex) { mutex_.lock(); }
  ~LockGuard() { mutex_.unlock(); }

  LockGuard(const LockGuard&) = delete;
  LockGuard& operator=(const LockGuard&) = delete;
};
#endif

/// @brief Use RAII construct to lock/unlock the `mutex`
/// @see LockGuard
/// @ingroup core
#define SEQUOIA_LOCK_GUARD(mutex) SEQUOIA_LOCK_GUARD_IMPL(mutex, __FILE__, __LINE__)

#ifdef SEQUOIA_DEBUG_THREADS
#define SEQUOIA_LOCK_GUARD_IMPL(mutex, file, line)                                                 \
  sequoia::core::LockGuard<std::decay<decltype(mutex)>::type> sequoia_LockGuard_##__LINE__(        \
      mutex, file, line)
#else
#define SEQUOIA_LOCK_GUARD_IMPL(mutex, file, line)                                                 \
  sequoia::core::LockGuard<std::decay<decltype(mutex)>::type> sequoia_LockGuard_##__LINE__(mutex)
#endif

/// @brief Lock the `mutex` using `std::unqiue_lock`
///
/// @param lock     Object of type `std::uniuqe_lock`
/// @param mutex    Mutex used to construct the `std::unique_lock`
///
/// @ingroup core
#define SEQUOIA_UNIQUE_LOCK(lock, mutex) SEQUOIA_UNIQUE_LOCK_IMPL(lock, mutex, __FILE__, __LINE__)

#ifdef SEQUOIA_DEBUG_THREADS
#define SEQUOIA_UNIQUE_LOCK_IMPL(lock, mutex, __FILE__, __LINE__)                                  \
  LOG(DEBUG) << "UNIQUE_LOCK: Constructing unique lock: " << file << ":" << line << " ...";        \
  std::unique_lock<std::decay<decltype(mutex)>::type> lock(mutex);                                 \
  LOG(DEBUG) << "UNIQUE_LOCK: Successfully constructed unique lock: " << file << ":" << line
#else
#define SEQUOIA_UNIQUE_LOCK_IMPL(lock, mutex, __FILE__, __LINE__)                                  \
  std::unique_lock<std::decay<decltype(mutex)>::type> lock(mutex)
#endif

} // namespace core

using Mutex = core::Mutex;
using SpinMutex = core::SpinMutex;

} // namespace sequoia

#endif
