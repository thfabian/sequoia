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

#ifndef SEQUOIA_RENDER_RENDERSERVER_H
#define SEQUOIA_RENDER_RENDERSERVER_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Mutex.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/RenderSystemObject.h"
#include <functional>
#include <thread>

namespace sequoia {

namespace render {

namespace internal {

/// @brief Ressource render task with the ability to block the calling thread
class RessourceTask {

  /// Check if task has been ran
  bool done_;

  /// Function to execute
  std::function<void(void)> function_;

  /// Mutex used to modify the `waitVariable`
  std::mutex mutex_;

  /// Condition variable to block the caller
  std::condition_variable waitVariable_;

public:
  RessourceTask(std::function<void(void)>&& function)
      : done_(false), function_(std::move(function)) {}

  /// @brief Block the calling thread
  void block() {
    SEQUOIA_UNIQUE_LOCK(lock, mutex_);
    waitVariable_.wait(lock, [this]() { return done_ == true; });
  }

  /// @brief Run the task
  void run() {
    function_();
    waitVariable_.notify_all();
    done_ = true;
  }
};

} // namespace internal

/// @brief Server of the render-system which handles all interaction with the backend render system
///
/// @ingroup render
class SEQUOIA_API RenderServer : public RenderSystemObject, public NonCopyable {
private:
public:
  RenderServer(RenderSystemKind renderSystemKind);

  /// @brief Spawn a ressource task
  void spawnRessourceTask(std::function<void(void)>&& function);

  // void spawnRessourceTaskAndWait(std::function<void(void)>&& task);
};

} // namespace render

} // namespace sequoia

#endif
