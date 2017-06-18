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

#ifndef SEQUOIA_GAME_SCENENODEALLOC_H
#define SEQUOIA_GAME_SCENENODEALLOC_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Memory.h"
#include "sequoia/Core/NonCopyable.h"
#include <memory>

namespace sequoia {

namespace game {

/// @brief Memory allocation routines used by SceneNode and related objects
/// @ingroup game
class SEQUOIA_API SceneNodeAlloc : public NonCopyable {
public:
  /// @brief Memory alloctor for SceneNode related objects
  template <class T>
  using AllocatorType = memory::cache_aligned_allocator<T>;

  /// @brief Create an object of type `T` (using AllocatorType) with `args...`
  template <class T, class... Args>
  static std::shared_ptr<T> create(Args&&... args) {
    return std::allocate_shared<T, AllocatorType<T>>(AllocatorType<T>(),
                                                     std::forward<Args>(args)...);
  }
};

} // namespace game

} // namespace sequoia

#endif
