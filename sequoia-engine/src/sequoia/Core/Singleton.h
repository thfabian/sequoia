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

#ifndef SEQUOIA_CORE_SINGLETON_H
#define SEQUOIA_CORE_SINGLETON_H

#include "sequoia/Core/NonCopyable.h"
#include <cassert>

namespace sequoia {

namespace core {

/// @brief CRTP for creating single-instance global classes (Singletons)
///
/// To use this class, derive from `Singleton` and define the member
/// `SEQUOIA_DECLARE_SINGLETON(...)`.
///
/// @bExample:
/// @code
///   // Foo.h
///   namespace sequoia {
///   namespace foo {
///
///   class Foo : public Singleton<Foo> {
///     // ...
///   };
///
///   }
///   }
///
///   // Foo.cpp
///   namespace sequoia {
///   SEQUOIA_DECLARE_SINGLETON(foo::Foo);
///   }
/// @endcode
///
/// Note that you **must** declare `SEQUOIA_DECLARE_SINGLETON` inside the `sequoia` namespace (and
/// not in any descendant namespaces) and fully qualify your class `Class` with respect to the
/// namespace `sequoia`.
/// @ingroup core
template <class Derived>
class Singleton : public NonCopyable {
public:
  static Derived* Instance;

public:
  Singleton() {
    assert(!Instance && "multiple allocation of Singleton");
    Instance = static_cast<Derived*>(this);
  }

  ~Singleton() {
    assert(Instance && "deleting non-existing Singleton");
    Instance = nullptr;
  }

  static Derived& getSingleton() {
    assert(Instance && "accessing non-existing Singleton");
    return *Instance;
  }

  static Derived* getSingletonPtr() { return Instance; }
};

} // namespace core

template <class Derived>
using Singleton = core::Singleton<Derived>;

} // namespace sequoia

// This should never be instantiated
template <typename T>
T* sequoia::core::Singleton<T>::Instance = nullptr;

/// @brief Declares the instance of the singleton `Class`
///
/// Note that you **must** declare `SEQUOIA_DECLARE_SINGLETON` inside the `sequoia` namespace (and
/// not in any descendant namespaces) and fully qualify your class `Class` with respect to the
/// namespace `sequoia`.
/// @ingroup core
#define SEQUOIA_DECLARE_SINGLETON(Class)                                                           \
  template <>                                                                                      \
  Class* sequoia::core::Singleton<Class>::Instance = nullptr

#endif
