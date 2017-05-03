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

#ifndef SEQUOIA_CORE_LISTENABLE_H
#define SEQUOIA_CORE_LISTENABLE_H

#include <set>
#include <tuple>

namespace sequoia {

namespace core {

namespace internal {

/// @brief List of listener
template <class ListenerType>
class ListenerList {
public:
  using ListenerListType = std::set<ListenerType*>;

  /// @brief Add a listener
  /// @returns `true` if the listener was successfully added, `false` otherwise
  bool addListener(ListenerType* listener) noexcept { return listeners_.insert(listener).second; }

  /// @brief Remove a listener
  /// @returns `true` if the listener was successfully removed, `false` otherwise
  bool removeListener(ListenerType* listener) noexcept { return listeners_.erase(listener); }

  ListenerListType& getListeners() noexcept { return listeners_; }
  const ListenerListType& getListeners() const noexcept { return listeners_; }

  /// @brief Get number of registered listeners
  typename ListenerListType::size_type getNumListeners() const noexcept {
    return listeners_.size();
  }

private:
  ListenerListType listeners_;
};

} // namespace internal

/// @brief Store a list of listeners which can listen to the inherited object
///
/// This class can be used to implement the Listener pattern.
/// @ingroup core
template <class... Listeners>
class Listenable {
  std::tuple<internal::ListenerList<Listeners>...> listenerList_;

public:
  /// @brief Add a listener of type `ListenerType` (this does @b not take ownership of the object)
  /// @returns `true` if the listener was successfully added, `false` otherwise
  template <class ListenerType>
  bool addListener(ListenerType* listener) noexcept {
    return get<ListenerType>().addListener(listener);
  }

  /// @brief Remove a listener of type `ListenerType`
  /// @returns `true` if the listener was successfully removed, `false` otherwise
  template <class ListenerType>
  bool removeListener(ListenerType* listener) noexcept {
    return get<ListenerType>().removeListener(listener);
  }

  /// @brief Get the number of registered listener of type `ListenerType`
  template <class ListenerType>
  std::size_t getNumListener() const noexcept {
    return get<ListenerType>().getNumListeners();
  }

  /// @brief Get a all the registered listeners of type `ListenerType`
  ///
  /// To iterate the listeners:
  /// @code{.cpp}
  ///   for(auto* listener : getListeners<FooListener>()) {
  ///     ...
  ///   }
  /// @endcode
  /// @{
  template <class ListenerType>
  typename internal::ListenerList<ListenerType>::ListenerListType& getListeners() noexcept {
    return get<ListenerType>().getListeners();
  }

  template <class ListenerType>
  const typename internal::ListenerList<ListenerType>::ListenerListType& getListeners() const
      noexcept {
    return get<ListenerType>().getListeners();
  }
  /// @}

private:
  /// @brief Get a `ListenerList` in the tuple by `ListenerType`
  template <class ListenerType>
  internal::ListenerList<ListenerType>& get() noexcept {
    return std::get<internal::ListenerList<ListenerType>>(listenerList_);
  }

  template <class ListenerType>
  const internal::ListenerList<ListenerType>& get() const noexcept {
    return std::get<internal::ListenerList<ListenerType>>(listenerList_);
  }
};

} // namespace core

template <class... Listeners>
using Listenable = core::Listenable<Listeners...>;

} // namespace sequoia

#endif
