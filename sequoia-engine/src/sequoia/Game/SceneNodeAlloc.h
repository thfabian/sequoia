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

namespace sequoia {

namespace game {

/// @namespace scene
/// @brief Various types an memory allocation routines for scene realted objects
/// @ingroup game
namespace scene {

/// @brief Memory alloctor for SceneNode related objects
/// @ingroup game
template <class T>
using allocator = memory::cache_aligned_allocator<T>;

/// @brief Allocate a shared object of type `T` with `args...`
/// @ingroup game
template <class T, class... Args>
inline std::shared_ptr<T> allocate_shared(Args&&... args) {
  return std::allocate_shared<T>(allocator<T>(), std::forward<Args>(args)...);
}

} // namespace scene

} // namespace game

} // namespace sequoia

#endif
