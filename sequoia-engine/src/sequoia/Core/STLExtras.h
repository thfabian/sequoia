//===-- sequoia/Core/String.h -------------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// This file contains some templates that are useful if you are working with the STL at all.
///
/// No library is required when using these functions
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_STLEXTRAS
#define SEQUOIA_CORE_STLEXTRAS

#include <algorithm>
#include <functional>
#include <utility>

namespace sequoia {

/// @addtogroup core
/// @{

namespace core {

//===------------------------------------------------------------------------------------------===//
//     Extra additions to <functional>
//===------------------------------------------------------------------------------------------===//

/// @brief An efficient, type-erasing, non-owning reference to a callable
/// 
/// This is intended for use as the type of a function parameter that is not used after the function
/// in question returns. This class does not own the callable, so it is not in general safe to store
/// a function_ref.
template<typename Fn> class function_ref;

template<typename Ret, typename ...Params>
class function_ref<Ret(Params...)> {
  Ret (*callback)(intptr_t callable, Params ...params);
  intptr_t callable;

  template<typename Callable>
  static Ret callback_fn(intptr_t callable, Params ...params) {
    return (*reinterpret_cast<Callable*>(callable))(
        std::forward<Params>(params)...);
  }

public:
  template <typename Callable>
  function_ref(Callable &&callable,
               typename std::enable_if<
                   !std::is_same<typename std::remove_reference<Callable>::type,
                                 function_ref>::value>::type * = nullptr)
      : callback(callback_fn<typename std::remove_reference<Callable>::type>),
        callable(reinterpret_cast<intptr_t>(&callable)) {}
  Ret operator()(Params ...params) const {
    return callback(callable, std::forward<Params>(params)...);
  }
};

} // namespace core

/// @}

} // namespace sequoia

#endif
