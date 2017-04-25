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
/// To use this class, derive from `Singleton` and define the member `Singleton::Instance`.
/// 
/// @bExample:
/// @code
///   // Foo.h
///   class Foo : public Singleton<Foo> {
///     // ...
///   };
///
///   // Foo.cpp
///   template<>
///   Foo* Singleton<Foo>::Instance = nullptr;
/// @endcode
/// @ingroup core
template<class Derived>
class Singleton : public NonCopyable {
protected:
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

  static Derived* getSingletonPtr() {
    return Instance; 
  }
};

} // namespace core

template<class Derived>
using Singleton = core::Singleton<Derived>;

} // namespace sequoia

#endif
