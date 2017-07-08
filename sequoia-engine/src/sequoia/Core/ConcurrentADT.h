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

#ifndef SEQUOIA_CORE_CONCURRENTADT_H
#define SEQUOIA_CORE_CONCURRENTADT_H

#include <tbb/concurrent_hash_map.h>

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

/// @brief Concurrent (thread-safe) version of `std::unordered_map`
template <typename Key, typename T>
using concurrent_unordered_map = tbb::concurrent_hash_map<Key, T>;

/// @}

} // namespace core

template <typename Key, typename T>
using concurrent_unordered_map = core::concurrent_unordered_map<Key, T>;

} // namespace sequoia

#endif
