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

#ifndef SEQUOIA_CORE_SINGLETONMANAGER_H
#define SEQUOIA_CORE_SINGLETONMANAGER_H

#include "sequoia/Core/Any.h"
#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Export.h"
#include "sequoia/Core/Singleton.h"
#include <list>
#include <memory>

namespace sequoia {

namespace core {

/// @brief The SingletonManager keeps track of the lifetime of all Singletons
///
/// The destructor deallocates all registered Singletons.
///
/// @ingroup core
class SEQUOIA_CORE_API SingletonManager : public Singleton<SingletonManager> {
  using ListType = std::list<any>;
  ListType singletons_;

public:
  /// @brief Allocate a new singleton of type `T` and return a pointer to it
  template <class T, typename... Args>
  T* allocateSingleton(Args&&... args) {
    SEQUOIA_ASSERT(hasSingleton<T>() == singletons_.end());
    singletons_.push_back(any(std::make_shared<T>(std::forward<Args>(args)...)));
    return any_cast<std::shared_ptr<T>>(singletons_.back()).get();
  }

  /// @brief Free all singletons (in reverse order they were registered)
  ~SingletonManager() {
    while(!singletons_.empty())
      singletons_.pop_back();
  }

  /// @brief Free the singleton given by type `T`
  /// @return `true` iff a Singleton of type `T` was successfully freed, `false` otherwise
  template <class T>
  bool freeSingleton() {
    auto it = hasSingleton<T>();
    if(it != singletons_.end()) {
      singletons_.erase(it);
      return true;
    }
    return false;
  }

  /// @brief Number of registered Singletons
  std::size_t numSingletons() const noexcept { return singletons_.size(); }

private:
  /// @brief Search the list for the Singleton of type `T` and return an iterator to it if found,
  /// otherwise it returns an iterator to std::list<any>::end()
  template <class T>
  ListType::iterator hasSingleton() {
    auto it = singletons_.begin();
    for(; it != singletons_.end(); ++it)
      if(any_cast<std::shared_ptr<T>>(&(*it)))
        return it;
    return it;
  }
};

} // namespace core

using SingletonManager = core::SingletonManager;

} // namespace sequoia

#endif
