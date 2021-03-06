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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERSERVER_H
#define SEQUOIA_ENGINE_RENDER_RENDERSERVER_H

#include "sequoia-engine/Core/ConcurrentADT.h"
#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Future.h"
#include "sequoia-engine/Core/Mutex.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Core/STLExtras.h"
#include <atomic>
#include <condition_variable>
#include <thread>

namespace sequoia {

namespace render {

/// @brief Server of the render-system which handles all interaction with the backend render system
///
/// @ingroup render
class SEQUOIA_API RenderServer : public NonCopyable {
public:
  RenderServer();
  ~RenderServer();

  /// @brief Spawn a ressource task
  template <class Functor>
  auto spawnRessourceTask(Functor&& function) {
    using ReturnType = typename core::function_return_t<decltype(function)>;

    auto task = std::make_shared<FutureTask<ReturnType>>(std::move(function));
    ressourceCtx_.Queue.push(std::static_pointer_cast<Task>(task));
    ressourceCtx_.TasksAvailable.notify_all();
    return task->getFuture();
  }

  /// @brief Initialize the ressource thread
  /// @param initFunction   Function executed by the ressource thread at startup (use this to bind
  ///                       the context of the render-system for example)
  void initRessourceThread(std::function<void(void)> initFunction);

private:
  struct QueueContext {
    /// Internal queue for tasks
    concurrent_queue<std::shared_ptr<Task>> Queue;

    /// Handle of the thread
    std::thread Thread;

    /// Condition variable used to wait if the queue is empty i.e clients wake the worker thread up
    /// via this variable
    std::condition_variable TasksAvailable;

    /// Mutex to modify `TasksAvailable`
    std::mutex Mutex;

    /// Signal the thread it should exit the function and join the main thread
    std::atomic<bool> Join;
  };

  /// @brief Implementation of the working threads
  static void runImpl(QueueContext* ctx);

private:
  /// Context of the ressource tasks
  QueueContext ressourceCtx_;
};

} // namespace render

} // namespace sequoia

#endif
