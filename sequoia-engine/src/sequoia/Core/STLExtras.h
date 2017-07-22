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

#ifndef SEQUOIA_CORE_STLEXTRAS_H
#define SEQUOIA_CORE_STLEXTRAS_H

#include "sequoia/Core/Compiler.h"
#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

#ifdef SEQUOIA_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4800)
#endif

namespace sequoia {

namespace core {

//===------------------------------------------------------------------------------------------===//
//     Extra additions to <functional>
//===------------------------------------------------------------------------------------------===//

/// @brief An efficient, type-erasing, non-owning reference to a callable
///
/// This is intended for use as the type of a function parameter that is not used after the function
/// in question returns. This class does not own the callable, so it is not in general safe to store
/// a function_ref.
///
/// @ingroup core
/// @{
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
/// @}

namespace internal {

template <class T, typename... Args>
constexpr auto get_return_type(T&& f, Args&&... args) -> decltype(f(args...)) {
  return f(args...);
}

template <class T>
constexpr auto get_return_type(T&& f) -> decltype(f()) {
  return f();
}

} // namespace internal

/// @typedef function_return_t
/// @brief Get the return type of *any* invokable object (including lambda functions)
///
/// @b Example:
/// @code{.cpp}
///   auto fun = [](int a, float b) { return float(a + b); };
///   static_assert(std::is_same<float, function_return_t<decltype(fun), int, float>>::value, "");
/// @endcode
/// @ingroup core
template <typename FunctionType, typename... Args>
using function_return_t =
    decltype(internal::get_return_type(std::declval<FunctionType>(), std::declval<Args>()...));

namespace internal {

template <typename Ret, typename Arg, typename... Rest>
Arg first_argument_impl(Ret (*)(Arg, Rest...));

template <typename Ret, typename F, typename Arg, typename... Rest>
Arg first_argument_impl(Ret (F::*)(Arg, Rest...));

template <typename Ret, typename F, typename Arg, typename... Rest>
Arg first_argument_impl(Ret (F::*)(Arg, Rest...) const);

template <typename F>
decltype(first_argument_impl(&F::operator())) first_argument_impl(F);

} // namespace internal

/// @typedef function_first_argument_t
/// @brief Get the type of the first arugment of *any* invokable object
///
/// @code{.cpp}
///   auto foo = [](float f) {};
///   static_assert(std::is_same<float, function_first_argument_t<decltype(foo)>>::value, "");
/// @endcode
/// @ingroup core
template <typename FunctionType>
using function_first_argument_t =
    decltype(internal::first_argument_impl(std::declval<FunctionType>()));

//===------------------------------------------------------------------------------------------===//
//     Extra additions for arrays
//===------------------------------------------------------------------------------------------===//

/// @brief Get length of an array
/// @ingroup core
template <class T, std::size_t N>
constexpr inline size_t array_lengthof(T (&)[N]) {
  return N;
}

/// @brief Make `std::array`
/// @ingroup core
template <typename... T>
constexpr auto make_array(T&&... values)
    -> std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)> {
  return std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>{
      std::forward<T>(values)...};
}

//===------------------------------------------------------------------------------------------===//
//     Extra additions to <algorithm>
//===------------------------------------------------------------------------------------------===//

/// @brief  For a container of pointers, deletes the pointers and then clears the container
/// @ingroup core
template <typename Container>
void deleteContainerPointers(Container& C) {
  for(auto V : C)
    delete V;
  C.clear();
}

/// @brief In a container of pairs (usually a map) whose second element is a pointer, deletes the
/// second elements and then clears the container
/// @ingroup core
template <typename Container>
void deleteContainerSeconds(Container& C) {
  for(auto& V : C)
    delete V.second;
  C.clear();
}

/// @brief Provide wrappers to std::all_of which take ranges instead of having to pass begin/end
/// explicitly
/// @ingroup core
template <typename R, typename UnaryPredicate>
bool all_of(R&& Range, UnaryPredicate P) {
  return std::all_of(std::begin(Range), std::end(Range), P);
}

/// @brief Provide wrappers to std::any_of which take ranges instead of having to pass begin/end
/// explicitly
/// @ingroup core
template <typename R, typename UnaryPredicate>
bool any_of(R&& Range, UnaryPredicate P) {
  return std::any_of(std::begin(Range), std::end(Range), P);
}

/// @brief Provide wrappers to std::none_of which take ranges instead of having to pass begin/end
/// explicitly
/// @ingroup core
template <typename R, typename UnaryPredicate>
bool none_of(R&& Range, UnaryPredicate P) {
  return std::none_of(std::begin(Range), std::end(Range), P);
}

/// @brief  Provide wrappers to std::find which take ranges instead of having to pass begin/end
/// explicitly
/// @ingroup core
template <typename R, typename T>
auto find(R&& Range, const T& Val) -> decltype(std::begin(Range)) {
  return std::find(std::begin(Range), std::end(Range), Val);
}

/// @brief Provide wrappers to std::find_if which take ranges instead of having to pass begin/end
/// explicitly
/// @ingroup core
template <typename R, typename UnaryPredicate>
auto find_if(R&& Range, UnaryPredicate P) -> decltype(std::begin(Range)) {
  return std::find_if(std::begin(Range), std::end(Range), P);
}

template <typename R, typename UnaryPredicate>
auto find_if_not(R&& Range, UnaryPredicate P) -> decltype(std::begin(Range)) {
  return std::find_if_not(std::begin(Range), std::end(Range), P);
}

/// @brief Provide wrappers to std::remove_if which take ranges instead of having to pass begin/end
/// explicitly
/// @ingroup core
template <typename R, typename UnaryPredicate>
auto remove_if(R&& Range, UnaryPredicate P) -> decltype(std::begin(Range)) {
  return std::remove_if(std::begin(Range), std::end(Range), P);
}

/// @brief  Wrapper function around std::find to detect if an element exists in a container
/// @ingroup core
template <typename R, typename E>
bool is_contained(R&& Range, const E& Element) {
  return std::find(std::begin(Range), std::end(Range), Element) != std::end(Range);
}

/// @brief  Wrapper function around std::count to count the number of times an element @p Element
/// occurs in the given range @p Range
/// @ingroup core
template <typename R, typename E>
auto count(R&& Range, const E& Element) ->
    typename std::iterator_traits<decltype(std::begin(Range))>::difference_type {
  return std::count(std::begin(Range), std::end(Range), Element);
}

/// @brief  Wrapper function around std::count_if to count the number of times an element
/// satisfying a given predicate occurs in a range
/// @ingroup core
template <typename R, typename UnaryPredicate>
auto count_if(R&& Range, UnaryPredicate P) ->
    typename std::iterator_traits<decltype(std::begin(Range))>::difference_type {
  return std::count_if(std::begin(Range), std::end(Range), P);
}

/// @brief  Wrapper function around std::transform to apply a function to a range and store the
/// result elsewhere
/// @ingroup core
template <typename R, typename OutputIt, typename UnaryPredicate>
OutputIt transform(R&& Range, OutputIt d_first, UnaryPredicate P) {
  return std::transform(std::begin(Range), std::end(Range), d_first, P);
}

//===------------------------------------------------------------------------------------------===//
//     Extra additions to <type_tratis>
//===------------------------------------------------------------------------------------------===//

/// @brief If `T` is a pointer, just return it. If it is not, return `T&`
/// @ingroup core
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
/// @ingroup core
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

//===------------------------------------------------------------------------------------------===//
//     Extra additions to <tuple>
//===------------------------------------------------------------------------------------------===//

/// @brief Check if type `T` is in the tuple
/// @ingroup core
/// @{
template <typename T, typename Tuple>
struct tuple_has_type;

template <typename T>
struct tuple_has_type<T, std::tuple<>> : std::false_type {};

template <typename T, typename U, typename... Ts>
struct tuple_has_type<T, std::tuple<U, Ts...>> : tuple_has_type<T, std::tuple<Ts...>> {};

template <typename T, typename... Ts>
struct tuple_has_type<T, std::tuple<T, Ts...>> : std::true_type {};
/// @}

template <typename Tuple, typename T>
constexpr bool tuple_has_value_impl(Tuple const&, T&&, std::index_sequence<>) {
  return false;
}

template <typename Tuple, typename T, size_t first, size_t... is>
constexpr bool tuple_has_value_impl(Tuple const& t, T&& value, std::index_sequence<first, is...>) {
  return (std::get<first>(t) == value) ||
         tuple_has_value_impl(t, value, std::index_sequence<is...>{});
}

/// @brief Check if `value` is in the tuple
///
/// Requires `value` to be equality comparable with all values in the tuple.
///
/// @ingroup core
template <typename... Elements, typename T>
constexpr bool tuple_has_value(std::tuple<Elements...> const& t, T&& value) {
  return tuple_has_value_impl(t, value, std::index_sequence_for<Elements...>{});
}

} // namespace core

} // namespace sequoia

#ifdef SEQUOIA_COMPILER_MSVC
#pragma warning(pop)
#endif

#endif
