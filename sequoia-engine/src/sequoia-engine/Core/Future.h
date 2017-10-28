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

#ifndef SEQUOIA_ENGINE_CORE_FUTURE_H
#define SEQUOIA_ENGINE_CORE_FUTURE_H

#include "sequoia-engine/Core/Assert.h"
#include "sequoia-engine/Core/Exception.h"
#include "sequoia-engine/Core/Mutex.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Core/Optional.h"
#include "sequoia-engine/Core/Task.h"
#include <condition_variable>
#include <functional>
#include <memory>

namespace sequoia {

namespace core {

namespace internal {

template <class T>
class FutureResult {
  core::optional<T> value_;

public:
  template <class S>
  void set(S&& value) {
    value_ = std::forward<S>(value);
  }
  T& get() {
    SEQUOIA_ASSERT_MSG(isReady(), "value not ready");
    return *value_;
  }

  bool isReady() const noexcept { return value_.is_initialized(); }
};

template <>
class FutureResult<void> {
  bool isReady_;

public:
  void set() { isReady_ = true; }
  void get() { SEQUOIA_ASSERT_MSG(isReady(), "value not ready"); }

  bool isReady() const noexcept { return isReady_; }
};

template <class T>
class FutureStateBase : public NonCopyable {
protected:
  /// Store the result of the future
  FutureResult<T> result_;

  /// Store the error (if any)
  std::exception_ptr error_;

  /// Wait variable
  mutable std::condition_variable available_;

  /// Access mutex
  mutable std::mutex mutex_;

public:
  /// @brief Notify all waiting threads
  void notifyAll() const { available_.notify_all(); }

  /// @brief Set the `error` and notify waiting variables
  void setError(std::exception_ptr error) {
    error_ = error;
    notifyAll();
  }

  /// @brief Blocks until the result becomes available
  void wait() const {
    std::unique_lock<std::mutex> lock(mutex_);
    available_.wait(lock, [this]() -> bool { return result_.isReady() || error_; });
  }

  /// @brief Is the result ready?
  bool isReady() const noexcept { return result_.isReady(); }
};

template <class T>
class FutureState : public FutureStateBase<T> {
public:
  using Base = FutureStateBase<T>;
  using Base::Base;

  /// @brief Set the `value` and notify all waiting threads
  template <class S>
  void setValue(S&& value, bool notify = true) {
    this->result_.set(std::forward<S>(value));
    if(notify)
      this->notifyAll();
  }

  /// @brief Waits until the future has a valid result and retrieves it
  T& get() {

    // Wait for task to finish
    this->wait();

    // Rethrow exception if an error occured
    if(this->error_)
      std::rethrow_exception(this->error_);

    // Retrive the value
    if(this->isReady())
      return this->result_.get();

    SEQUOIA_THROW(core::Exception, "future is not initailized");
  }
};

template <>
class FutureState<void> : public FutureStateBase<void> {
public:
  using Base = FutureStateBase<void>;
  using Base::Base;

  /// @brief Set the result to be ready and notify all waiting threads
  void setValue(bool notify = true) {
    this->result_.set();
    if(notify)
      this->notifyAll();
  }

  /// @brief Waits until the future has a valid result
  void get() {

    // Wait for task to finish
    this->wait();

    // Rethrow exception if an error occured
    if(this->error_)
      std::rethrow_exception(this->error_);

    // Retrive the value
    if(this->isReady())
      return;

    SEQUOIA_THROW(core::Exception, "future is not initailized");
  }
};

} // namespace internal

template <class T>
class FutureBase {
protected:
  std::shared_ptr<internal::FutureState<T>> state_; ///< Shared state to store the value

public:
  /// @brief Create empty future
  FutureBase() : state_(nullptr){};

  /// @brief Create with a shared state
  FutureBase(std::shared_ptr<internal::FutureState<T>> state) : state_(std::move(state)) {}

  /// @brief Copy & Move constructor
  FutureBase(const FutureBase&) = default;
  FutureBase(FutureBase&&) = default;

  /// @brief Copy & Move assign
  FutureBase& operator=(const FutureBase&) = default;
  FutureBase& operator=(FutureBase&&) = default;

  /// @brief Blocks until the result becomes available
  void wait() const {
    if(!valid())
      return;
    state_->wait();
  }

  /// @brief Checks if the future refers to a shared state
  bool valid() const { return state_ != nullptr; }

  /// @brief Get the shared state (internal use only)
  const std::shared_ptr<internal::FutureState<T>>& getState() const { return state_; }
};

/// @brief Futures provide a mechanism to access the result of asynchronous operations
///
/// Futures support value semantics.
///
/// @ingroup core
template <class T>
class Future : public FutureBase<T> {
public:
  using Base = FutureBase<T>;
  using Base::Base;

  /// @brief Waits until the future has a valid result and retrieves it
  ///
  /// It effectively calls wait() in order to wait for the result. The behavior is undefined if
  /// `valid()` is `false` before the call to this function. Any shared state is released and
  /// `valid()` is `false` after a call to this method.
  ///
  /// @note If an error occured during the computation of the result, the execption is rethrown
  /// instead
  T get() {
    SEQUOIA_ASSERT_MSG(this->valid(),
                       "future does not contain a shared state or result was already queried");
    this->wait();
    auto oldState = this->state_;
    this->state_ = nullptr;
    return std::move(oldState->get());
  }

  /// @brief Create Future from existing `value`
  template <class S>
  static Future<T> create(S&& value) {
    auto state = std::make_shared<internal::FutureState<T>>();
    state->setValue(std::forward<S>(value), false);
    return Future<T>(std::move(state));
  }
};

template <>
class Future<void> : public FutureBase<void> {
public:
  using Base = FutureBase<void>;
  using Base::Base;

  void get() {
    SEQUOIA_ASSERT_MSG(this->valid(),
                       "future does not contain a shared state or result was already queried");
    this->wait();
    this->state_->get();
    this->state_ = nullptr;
  }
};

namespace internal {

template <class T>
struct RunAndAssign {
  static inline void apply(std::shared_ptr<internal::FutureState<T>>& state,
                           std::function<T(void)>& function) {
    state->setValue(function(), true);
  }
};

template <>
struct RunAndAssign<void> {
  static inline void apply(std::shared_ptr<internal::FutureState<void>>& state,
                           std::function<void(void)>& function) {
    function();
    state->setValue(true);
  }
};

} // namespace internal

/// @brief Specialization of a `Task` which produces a `Future`
/// @ingroup core
template <class T>
class FutureTask : public Task {
public:
  /// @brief Type of the task function
  using TaskFunction = std::function<T(void)>;

  /// @brief Initialize task with the `function` to run
  FutureTask(TaskFunction&& function)
      : state_(std::make_shared<internal::FutureState<T>>()), function_(std::move(function)) {}

  /// @brief Run the task
  virtual void run() override {
    try {
      internal::RunAndAssign<T>::apply(state_, function_);
    } catch(...) {
      state_->setError(std::current_exception());
    }
  }

  /// @brief Returns the associated Future future associated with the FutureTask result
  Future<T> getFuture() const { return Future<T>(state_); }

private:
  std::shared_ptr<internal::FutureState<T>> state_; ///< Associated future state
  TaskFunction function_;                           ///< Function to execute
};

} // namespace core

template <class T>
using Future = core::Future<T>;

template <class T>
using FutureTask = core::FutureTask<T>;

} // namespace sequoia

#endif
