//===-- sequoia/Core/Singleton.h ----------------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//
//
/// @file
/// Singleton base class.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_SINGLETON_H
#define SEQUOIA_CORE_SINGLETON_H

namespace sequoia {

namespace core {

/// @brief CRTP base of singleton classes
///
/// The singleton concept is a design pattern that restricts the instantiation of a class to one
/// object. The class will be created lazily on first usage.
///
/// @tparam Derived   Class to implement the singleton concept. The Derived class inherits the
///                   `instance_` static member which needs to be defined (see example below).
///
/// @b Example:
///
/// To declare the Singleton class (MySingleton.h):
///
/// @code
///   class MySingleton : public Singleton<MySingleton> {
///     // ...
///   };
/// @endcode
///
/// In addition, you need to provide a definition for the `instance_` member in a translation unit
/// (MySingleton.cpp).
///
/// @code
///   template<>
///   MySingleton* Singleton<MySingleton>::instance_ = nullptr;
/// @endcode
///
/// @ingroup core
template <class Derived>
class Singleton {
private:
  static Derived* instance_;

public:
  /// @brief Return the unique instance of the singleton class
  ///
  /// The class will be created on first use
  static Derived& instance() {
    if(!instance_)
      instance_ = new Derived();
    return (*instance_);
  }
};

} // namespace core

} // namespace sequoia

#endif
