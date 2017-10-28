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

#ifndef SEQUOIA_ENGINE_CORE_CONCURRENTADT_H
#define SEQUOIA_ENGINE_CORE_CONCURRENTADT_H

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_vector.h>

namespace sequoia {

namespace core {

/// @addtogroup core
/// @{

/// @brief Concurrent (thread-safe) version of a `std::unordered_map`
template <typename Key, typename T>
using concurrent_unordered_map = tbb::concurrent_unordered_map<Key, T>;

/// @brief Concurrent (thread-safe) version of a `std::vector`
template <typename T>
using concurrent_vector = tbb::concurrent_vector<T>;

/// @brief Concurrent (thread-safe) version of a queue
template <typename T>
using concurrent_queue = tbb::concurrent_queue<T>;

/// @}

} // namespace core

template <typename Key, typename T>
using concurrent_unordered_map = core::concurrent_unordered_map<Key, T>;

template <typename T>
using concurrent_vector = core::concurrent_vector<T>;

template <typename T>
using concurrent_queue = core::concurrent_queue<T>;

} // namespace sequoia

#endif
