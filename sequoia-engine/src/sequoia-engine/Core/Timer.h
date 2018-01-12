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

#ifndef SEQUOIA_ENGINE_CORE_TIMER_H
#define SEQUOIA_ENGINE_CORE_TIMER_H

#include "sequoia-engine/Core/Export.h"
#include <chrono>

namespace sequoia {

namespace core {

/// @brief High resolution timer
///
/// This class is used to time operations using the built-in clock type with the highest precision.
///
/// @code{.cpp}
///   Timer t;
///   // Some work ...
///   std::cout << "elapsed time: " << t.stop() << " ms." << std::endl;
/// @endcode
/// @ingroup core
class Timer {
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;

public:
  /// @brief Start the timer
  Timer() : start_(std::chrono::high_resolution_clock::now()) {}

  /// @brief Reset the timer to the current time
  inline void start() noexcept { start_ = std::chrono::high_resolution_clock::now(); }

  /// @brief Return the number of milliseconds elapsed since the timer was last reset via `start()`
  inline double stop() noexcept {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start_).count();
  }
};

} // namespace core

using Timer = core::Timer;

} // namespace sequoia

#endif
