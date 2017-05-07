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

#ifndef SEQUOIA_CORE_STLEXTRAS_H
#define SEQUOIA_CORE_STLEXTRAS_H

#include "sequoia/Core/Compiler.h"
#include <algorithm>
#include <functional>
#include <type_traits>
#include <utility>

#ifdef SEQUOIA_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4800)
#endif

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

//===------------------------------------------------------------------------------------------===//
//     Extra additions to <functional>
//===------------------------------------------------------------------------------------------===//

/// @brief An efficient, type-erasing, non-owning reference to a callable
///
/// This is intended for use as the type of a function parameter that is not used after the function
/// in question returns. This class does not own the callable, so it is not in general safe to store
/// a function_ref.
template <typename Fn>
class function_ref;

template <typename Ret, typename... Params>
class function_ref<Ret(Params...)> {
  Ret (*callback)(intptr_t callable, Params... params);
  intptr_t callable;

  template <typename Callable>
  static Ret callback_fn(intptr_t callable, Params... params) {
    return (*reinterpret_cast<Callable*>(callable))(std::forward<Params>(params)...);
  }

public:
  template <typename Callable>
  function_ref(Callable&& callable,
               typename std::enable_if<!std::is_same<typename std::remove_reference<Callable>::type,
                                                     function_ref>::value>::type* = nullptr)
      : callback(callback_fn<typename std::remove_reference<Callable>::type>),
        callable(reinterpret_cast<intptr_t>(&callable)) {}
  Ret operator()(Params... params) const {
    return callback(callable, std::forward<Params>(params)...);
  }
};

//===------------------------------------------------------------------------------------------===//
//     Extra additions for arrays
//===------------------------------------------------------------------------------------------===//

/// @brief Get length of an array
template <class T, std::size_t N>
constexpr inline size_t array_lengthof(T (&)[N]) {
  return N;
}

//===------------------------------------------------------------------------------------------===//
//     Extra additions to <algorithm>
//===------------------------------------------------------------------------------------------===//

/// @brief  For a container of pointers, deletes the pointers and then clears the container.
template <typename Container>
void deleteContainerPointers(Container& C) {
  for(auto V : C)
    delete V;
  C.clear();
}

/// @brief  In a container of pairs (usually a map) whose second element is a pointer, deletes the
/// second elements and then clears the container.
template <typename Container>
void deleteContainerSeconds(Container& C) {
  for(auto& V : C)
    delete V.second;
  C.clear();
}

/// @brief  Provide wrappers to std::all_of which take ranges instead of having to pass begin/end
/// explicitly
template <typename R, typename UnaryPredicate>
bool all_of(R&& Range, UnaryPredicate P) {
  return std::all_of(std::begin(Range), std::end(Range), P);
}

/// @brief  Provide wrappers to std::any_of which take ranges instead of having to pass begin/end
/// explicitly
template <typename R, typename UnaryPredicate>
bool any_of(R&& Range, UnaryPredicate P) {
  return std::any_of(std::begin(Range), std::end(Range), P);
}

/// @brief  Provide wrappers to std::none_of which take ranges instead of having to pass begin/end
/// explicitly
template <typename R, typename UnaryPredicate>
bool none_of(R&& Range, UnaryPredicate P) {
  return std::none_of(std::begin(Range), std::end(Range), P);
}

/// @brief  Provide wrappers to std::find which take ranges instead of having to pass begin/end
/// explicitly.
template <typename R, typename T>
auto find(R&& Range, const T& Val) -> decltype(std::begin(Range)) {
  return std::find(std::begin(Range), std::end(Range), Val);
}

/// @brief Provide wrappers to std::find_if which take ranges instead of having to pass begin/end
/// explicitly
template <typename R, typename UnaryPredicate>
auto find_if(R&& Range, UnaryPredicate P) -> decltype(std::begin(Range)) {
  return std::find_if(std::begin(Range), std::end(Range), P);
}

template <typename R, typename UnaryPredicate>
auto find_if_not(R&& Range, UnaryPredicate P) -> decltype(std::begin(Range)) {
  return std::find_if_not(std::begin(Range), std::end(Range), P);
}

/// @brief  Provide wrappers to std::remove_if which take ranges instead of having to pass begin/end
/// explicitly
template <typename R, typename UnaryPredicate>
auto remove_if(R&& Range, UnaryPredicate P) -> decltype(std::begin(Range)) {
  return std::remove_if(std::begin(Range), std::end(Range), P);
}

/// @brief  Wrapper function around std::find to detect if an element exists in a container
template <typename R, typename E>
bool is_contained(R&& Range, const E& Element) {
  return std::find(std::begin(Range), std::end(Range), Element) != std::end(Range);
}

/// @brief  Wrapper function around std::count to count the number of times an element @p Element
/// occurs in the given range \p Range
template <typename R, typename E>
auto count(R&& Range, const E& Element) ->
    typename std::iterator_traits<decltype(std::begin(Range))>::difference_type {
  return std::count(std::begin(Range), std::end(Range), Element);
}

/// @brief  Wrapper function around std::count_if to count the number of times an element
/// satisfying a given predicate occurs in a range
template <typename R, typename UnaryPredicate>
auto count_if(R&& Range, UnaryPredicate P) ->
    typename std::iterator_traits<decltype(std::begin(Range))>::difference_type {
  return std::count_if(std::begin(Range), std::end(Range), P);
}

/// @brief  Wrapper function around std::transform to apply a function to a range and store the
/// result elsewhere
template <typename R, typename OutputIt, typename UnaryPredicate>
OutputIt transform(R&& Range, OutputIt d_first, UnaryPredicate P) {
  return std::transform(std::begin(Range), std::end(Range), d_first, P);
}

//===------------------------------------------------------------------------------------------===//
//     Extra additions to <type_tratis>
//===------------------------------------------------------------------------------------------===//

/// @brief If `T` is a pointer, just return it. If it is not, return `T&`
/// @{
template <typename T, typename Enable = void>
struct add_lvalue_reference_if_not_pointer {
  typedef T& type;
};

template <typename T>
struct add_lvalue_reference_if_not_pointer<
    T, typename std::enable_if<std::is_pointer<T>::value>::type> {
  typedef T type;
};
/// @}

/// @brief If `T` is a pointer to `X`, return a pointer to const `X`. If it is not, return const `T`
/// @{
template <typename T, typename Enable = void>
struct add_const_past_pointer {
  typedef const T type;
};

template <typename T>
struct add_const_past_pointer<T, typename std::enable_if<std::is_pointer<T>::value>::type> {
  typedef const typename std::remove_pointer<T>::type* type;
};
/// @}

} // namespace core

} // namespace sequoia

#ifdef SEQUOIA_COMPILER_MSVC
#pragma warning(pop)
#endif

#endif
