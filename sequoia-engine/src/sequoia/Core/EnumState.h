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

#ifndef SEQUOIA_CORE_ENUMSTATE_H
#define SEQUOIA_CORE_ENUMSTATE_H

namespace sequoia {

namespace core {

namespace internal {

template <typename E, E first, E head>
void advanceEnum(E& v) {
  if(v == head)
    v = first;
}

template <typename E, E first, E head, E next, E... tail>
void advanceEnum(E& v) {
  if(v == head)
    v = next;
  else
    advanceEnum<E, first, next, tail...>(v);
}

} // namespace internal

/// @brief Allows implementation of cyclic state machine based on an `enum class`
///
/// @b Example
/// @code{.cpp}
///   enum class Fruit { Apple, Banana };
///
///   using FruitState = EnumState<Fruit, Fruit::Apple, Fruit::Banana>;
///
///   Fruit& operator++(Fruit& f) {
///     FruitState::advance(f);
///     return f;
///   }
///
///   int main() {
///     Fruit f = Fruit::Apple;
///     f++; // == Fruit::Banana
///     f++; // == Fruit::Apple
///   }
/// @endcode
///
/// @ingroup core
template <typename E, E first, E... values>
struct EnumState {
  static void advance(E& v) { internal::advanceEnum<E, first, first, values...>(v); }
};

} // namespace core

} // namespace sequoia

#endif
