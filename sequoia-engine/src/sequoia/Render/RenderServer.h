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

#ifndef SEQUOIA_RENDER_RENDERSERVER_H
#define SEQUOIA_RENDER_RENDERSERVER_H

#include "sequoia/Core/ConcurrentADT.h"
#include "sequoia/Core/Export.h"
#include "sequoia/Core/Mutex.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Core/STLExtras.h"
#include <atomic>
#include <functional>
#include <thread>

#include "sequoia/Core/Exception.h"
#include "sequoia/Core/Optional.h"
#include <future>
#include <iostream>

namespace sequoia {

namespace render {

// -----------------------------------
// TASK
// -----------------------------------

/// @brief Common task interface
/// @ingroup render
class Task : public NonCopyable {
public:
  /// @brief Deallocate the task
  virtual ~Task() {}

  /// @brief Run the task
  virtual void run() = 0;
};

// -----------------------------------
// FUTURE
// -----------------------------------

template <class T>
class FutureState : public NonCopyable {

  /// Store the result of the future
  core::optional<T> value_;

  /// Store the error (if any)
  std::exception_ptr error_;

  /// Wait variable
  mutable std::condition_variable available_;

  /// Access mutex
  mutable std::mutex mutex_;

public:
  ~FutureState() {}

  /// @brief Set the `value` and notify waiting threads
  template <class S>
  void setValue(S&& value) {
    value_ = std::forward<S>(value);
    available_.notify_all();
  }

  /// @brief Set the `error` and notify waiting variables
  void setError(std::exception_ptr error) {
    error_ = error;
    available_.notify_all();
  }

  /// @brief Waits until the future has a valid result and (depending on which template is used)
  /// retrieves it
  T& get() {

    // Wait for task to finish
    wait();

    // Rethrow exception if an error occured
    if(error_)
      std::rethrow_exception(error_);

    // Retrive the value
    if(value_)
      return *value_;

    SEQUOIA_THROW(core::Exception, "future is not initailized");
  }

  /// @brief Blocks until the result becomes available
  void wait() const {
    std::unique_lock<std::mutex> lock(mutex_);
    available_.wait(lock, [this]() -> bool { return value_ || error_; });
  }
};

template <class T>
class Future {
  std::shared_ptr<FutureState<T>> state_; ///< Shared state to store the value

public:
  /// @brief Create with a shared state
  Future(const std::shared_ptr<FutureState<T>>& state) : state_(state) {}

  /// @brief Copy & Move constructor
  Future(const Future&) = default;
  Future(Future&&) = default;
  
  /// @brief Copy & Move assign
  Future& operator=(const Future&) = default;
  Future& operator=(Future&&) = default;  

  /// @brief Waits until the future has a valid result and (depending on which template is used)
  /// retrieves it
  ///
  /// It effectively calls wait() in order to wait for the result. The behavior is undefined if
  /// `valid()` is `false` before the call to this function. Any shared state is released and
  /// `valid()` is `false` after a call to this method.
  ///
  /// @note If an error occured during the computation of the result, the execption is rethrown 
  /// instead
  T get() {
    SEQUOIA_ASSERT_MSG(valid(),
                       "future does not contain a shared state or result was already queried");
    wait();
    auto oldState = state_;
    state_ = nullptr;
    return std::move(oldState->get());
  }

  /// @brief Blocks until the result becomes available
  void wait() const { state_->wait(); }

  /// @brief Checks if the future refers to a shared state
  bool valid() const { return state_ != nullptr; }
};

namespace internal {

template <class T>
struct RunAndAssign {
  static inline void apply(std::shared_ptr<FutureState<T>>& state,
                           std::function<T(void)>& function) {
    state->setValue(function());
  }
};

//template <>
//struct RunAndAssign<void> {
//  static inline void apply(std::shared_ptr<FutureState<void>>& state,
//                           std::function<void(void)>& function) {
//    function();
//    state->setValue();
//  }
//};

} // namespace internal

template <class T>
class SEQUOIA_API FutureTask final : public Task {
public:
  /// @brief Type of the task function
  using TaskFunction = std::function<T(void)>;

  /// @brief Initialize task with the `function` to run
  FutureTask(TaskFunction&& function)
      : state_(std::make_shared<FutureState<T>>()), function_(std::move(function)) {}

  /// @brief Run the task
  virtual void run() override {
    try {
      internal::RunAndAssign<T>::apply(state_, function_);
    } catch(...) {
      state_->setError(std::current_exception());
    }
  }

  /// @brief Returns the shared state associated with the Future of the task
  const std::shared_ptr<FutureState<T>>& getState() const { return state_; }

  /// @brief Returns a future associated with the FutureTask result
  Future<T> getFuture() const { return Future<T>(state_); }

private:
  std::shared_ptr<FutureState<T>> state_; ///< Associated future state
  TaskFunction function_;                 ///< Function to execute
};

// -----------------------------------
// RENDER_SERVER
// -----------------------------------

/// @brief Server of the render-system which handles all interaction with the backend render system
///
/// @ingroup render
class SEQUOIA_API RenderServer : public NonCopyable {
public:
  RenderServer();
  ~RenderServer();

  /// @brief Spawn a ressource task
  template <class Functor>
  auto spawnRessourceTask(Functor&& function) {
    using ReturnType = typename core::function_return_t<decltype(function)>;
  
    auto task = std::make_shared<FutureTask<ReturnType>>(std::move(function));
  
    std::lock_guard<std::mutex> lock(ressourceCtx_.Mutex);
    ressourceCtx_.Queue.push(std::static_pointer_cast<Task>(task));
    ressourceCtx_.TasksAvailable.notify_all();
  
    return task->getFuture();
  }

  /// @brief Initialize the ressource thread
  /// @param initFunction   Function executed by the ressource thread at startup (use this to bind
  ///                       the context of the render-system for example)
  void initRessourceThread(std::function<void(void)> initFunction);

private:
  struct QueueContext {
    /// Internal queue for tasks
    concurrent_queue<std::shared_ptr<Task>> Queue;

    /// Handle of the thread
    std::thread Thread;

    /// Condition variable used to wait if the queue is empty i.e clients weak the worker thread up
    /// via this variable
    std::condition_variable TasksAvailable;

    /// Mutex to modify `WaitVar`
    std::mutex Mutex;

    /// Signal the thread it should exit the function
    std::atomic<bool> Join;
  };

  /// @brief Implementation of the working threads
  static void runImpl(QueueContext* ctx);

private:
  /// Context of the ressource tasks
  QueueContext ressourceCtx_;
};

} // namespace render

} // namespace sequoia

#endif
