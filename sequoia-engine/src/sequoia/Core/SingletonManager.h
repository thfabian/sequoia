//===-- sequoia/Core/SingletonManager.h ---------------------------------------------*- C++ -*-===//
//
//                                      S E Q U O I A
//
// This file is distributed under the MIT License (MIT).
// See LICENSE.txt for details.
//
//===------------------------------------------------------------------------------------------===//

#ifndef SEQUOIA_CORE_SINGLETONMANAGER_H
#define SEQUOIA_CORE_SINGLETONMANAGER_H

#include "sequoia/Core/Assert.h"
#include "sequoia/Core/Core.h"
#include <OGRE/OgreSingleton.h>
#include <boost/any.hpp>
#include <list>
#include <memory>

namespace sequoia {

namespace core {

/// @brief The SingletonManager keeps track of the lifetime of all Singletons
///
/// The destructor deallocates all registered Singletons.
///
/// @ingroup core
class SEQUOIA_CORE_EXPORT SingletonManager : public Ogre::Singleton<SingletonManager> {
  using list_type = std::list<boost::any>;

public:
  /// @brief Allocate a new singleton of type `T` and return a pointer to it
  template <class T, typename... Args>
  T* allocateSingleton(Args&&... args) {
    SEQUOIA_ASSERT(hasSingleton<T>() == singletons_.end());
    singletons_.push_back(boost::any(std::make_shared<T>(std::forward<Args>(args)...)));
    return boost::any_cast<std::shared_ptr<T>>(singletons_.back()).get();
  }

  /// @brief Free the singleton given by type `T`
  ///
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
  /// otherwise it returns an iterator to std::list<boost::any>::end()
  template <class T>
  list_type::iterator hasSingleton() {
    auto it = singletons_.begin();
    for(; it != singletons_.end(); ++it)
      if(boost::any_cast<std::shared_ptr<T>>(&(*it)))
        return it;
    return it;
  }

private:
  list_type singletons_;
};

} // namespace core

using SingletonManager = core::SingletonManager;

} // namespace sequoia

#endif
