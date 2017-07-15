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

#ifndef SEQUOIA_CORE_TASK_H
#define SEQUOIA_CORE_TASK_H

#include "sequoia/Core/NonCopyable.h"

namespace sequoia {

namespace core {

/// @brief Common interface tasks
/// @ingroup core
class Task : public NonCopyable {
public:
  /// @brief Deallocate the task
  virtual ~Task() {}

  /// @brief Run the task
  virtual void run() = 0;
};

} // namespace core

using Task = core::Task;

} // namespace sequoia

#endif
