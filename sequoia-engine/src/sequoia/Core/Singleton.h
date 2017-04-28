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

#ifndef SEQUOIA_CORE_SINGLETON_H
#define SEQUOIA_CORE_SINGLETON_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/NonCopyable.h"

namespace sequoia {

namespace core {

/// @brief CRTP for creating single-instance global classes (Singletons)
///
/// To use this class, derive from `Singleton` and define the member
/// `SEQUIOA_DECLARE_SINGLETON(...)`.
///
/// @bExample:
/// @code
///   // Foo.h
///   namespace sequioa {
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
///   namespace sequioa {
///   SEQUIOA_DECLARE_SINGLETON(foo::Foo);
///   }
/// @endcode
///
/// Note that you **must** declare `SEQUIOA_DECLARE_SINGLETON` inside the `sequioa` namespace (and
/// not in any descendant namespaces) and fully qualify your class `Class` with respect to the
/// namespace `sequioa`.
/// @ingroup core
template <class Derived>
class Singleton : public NonCopyable {
public:
  static Derived* Instance;

public:
  Singleton() {
    SEQUOIA_ASSERT(!Instance);
    Instance = static_cast<Derived*>(this);
  }

  ~Singleton() {
    SEQUOIA_ASSERT(Instance);
    Instance = nullptr;
  }

  static Derived& getSingleton() {
    SEQUOIA_ASSERT(Instance);
    return *Instance;
  }

  static Derived* getSingletonPtr() { return Instance; }
};

} // namespace core

template <class Derived>
using Singleton = core::Singleton<Derived>;

} // namespace sequoia

/// @macro SEQUIOA_DECLARE_SINGLETON
/// @brief Declares the instance of the singleton `Class`
///
/// Note that you **must** declare `SEQUIOA_DECLARE_SINGLETON` inside the `sequioa` namespace (and
/// not in any descendant namespaces) and fully qualify your class `Class` with respect to the
/// namespace `sequioa`.
/// @ingroup core
#define SEQUIOA_DECLARE_SINGLETON(Class)                                                           \
  template <>                                                                                      \
  Class* sequoia::core::Singleton<Class>::Instance = nullptr

#endif
