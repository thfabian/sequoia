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

#ifndef SEQUOIA_CORE_MICROBENCHMARK_H
#define SEQUOIA_CORE_MICROBENCHMARK_H

#include "sequoia/Core/Export.h"
#include <memory>

#ifdef MIB_OFF

#define MIB_START(name) ((void)0)
#define MIB_STOP(name) ((void)0)
#define MIB_NEXT(name) ((void)0)
#define MIB_PRINT(name) ((void)0)

#else

#define MIB_START(name)                                                                            \
  do {                                                                                             \
    if(sequoia::core::MicroBenchmarkerGlobal == nullptr)                                           \
      sequoia::core::MicroBenchmarker::init();                                                     \
    sequoia::core::MicroBenchmarkerGlobal->stopTimer();                                            \
    sequoia::core::MicroBenchmarkerGlobal->startBenchmark(name);                                   \
    sequoia::core::MicroBenchmarkerGlobal->startTimer();                                           \
  } while(0)

#define MIB_NEXT(name)                                                                             \
  do {                                                                                             \
    sequoia::core::MicroBenchmarkerGlobal->stopTimer();                                            \
    sequoia::core::MicroBenchmarkerGlobal->nextBenchmark(name);                                    \
    sequoia::core::MicroBenchmarkerGlobal->startTimer();                                           \
  } while(0)

#define MIB_STOP(name)                                                                             \
  do {                                                                                             \
    sequoia::core::MicroBenchmarkerGlobal->stopTimer();                                            \
    sequoia::core::MicroBenchmarkerGlobal->stopBenchmark(name);                                    \
    sequoia::core::MicroBenchmarkerGlobal->startTimer();                                           \
  } while(0)

#define MIB_PRINT(mode)                                                                            \
  do {                                                                                             \
    sequoia::core::MicroBenchmarkerGlobal->setTimerMode(mode);                                     \
    sequoia::core::MicroBenchmarkerGlobal->print();                                                \
  } while(0)

#endif

namespace sequoia {

namespace core {

/// @brief Micro benchmarking infrastrucure
///
/// The micro benchmarking infrastrucure uses the rdtsc timer for accurate measurement, hence cycle
/// measurements will only be correct for single threaded execution. The entire functionality is
/// encapsulated in a few macros.
///
///   - `MIB_START("name")`: Register an event, identified by `name` (given as a string, hence "."),
///     and start the timer. If this is called within an already existing event, the event will be
///     treated as a child node of the parent event.
///   - `MIB_STOP("name")`: Stop the event and timer given by `name`.
///   - `MIB_NEXT("name")`: Stop the last event and timer and register a new event `name`.
///   - `MIB_PRINT("mode")`: Prints all events in a tree like fashion to the console (`stdout`).
///     The `mode` can be either "cycle" (using rdtsc timer) or "nsec" (using STL chrono)
///     depending on the preferred time-unit.
///
/// The identifier `name` has to be unique as starting and stopping an event multiple times given
/// by the same name will be treated as the same event (See example).
///
/// @b Example
/// The following simple example shows how to measure the `std::sort` operation on a vector and
/// a list.
///
/// @code{.cpp}
///   #include "sequoia/Core/MicroBenchmark.h"
///   #include <algorithm>  // std::sort, std::generate
///   #include <vector>     // std::vector
///   #include <list>       // std::list
///   #include <random>     // std::mt19937
///
///   int main() {
///     std::vector<int> vec(1000);
///     std::list<int> list(1000);
///
///     // Fill containers with random numbers
///     std::random_device rnd_device;
///     std::mt19937 mersenne_engine(rnd_device());
///     std::generate(vec.begin(), vec.end(), mersenne_engine);
///     std::generate(list.begin(), list.end(), mersenne_engine);
///
///     // Profiler sorting over 1000 repetitions
///     for(int i=0; i < 1000; ++i) {
///        MIB_START("vector sort")
///        std::sort(vec.begin(), vec.end());
///        MIB_NEXT("list sort")
///        list.sort();
///        MIB_STOP("list sort")
///     }
///
///     MIB_PRINT("cycle")
///     return 0;
///   }
/// @endcode
///
/// this will result in something like:

/// @code
/// ================================================================================
/// ┬ (% of parent) name                           cycle_mean   spread    run_cnt
/// ├─  8% vector sort                            [      3495       3%       1000 ]
/// └─ 92% list sort                              [     38298       3%       1000 ]
/// @endcode
///
/// @ingroup core
class SEQUOIA_CORE_API MicroBenchmarker {
  class MicroBenchmarkerImpl;
  std::unique_ptr<MicroBenchmarkerImpl> impl_;

public:
  /// @brief Allocate and initialize the global
  static void init();

  /// @brief Start a new benchmark `name`
  void startBenchmark(const char* name);

  /// @brief Stop last benchmark and start next benchmark `name`
  void nextBenchmark(const char* name);

  /// @brief Stop the benchmark `name`
  void stopBenchmark(const char* name);

  /// @brief Start the global timer
  void startTimer();

  /// @brief Stop the global timer
  void stopTimer();

  /// @brief Set the timer mode, avaialbe options are `"nsec"` and `"cycle"`
  void setTimerMode(const char* mode);

  /// @brief Print the result to `stdout`
  void print();

private:
  MicroBenchmarker();
};

extern SEQUOIA_CORE_API MicroBenchmarker* MicroBenchmarkerGlobal;

} // namespace core

} // namespace sequoia

#endif
