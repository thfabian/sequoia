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

#include "sequoia-engine/Render/RenderServer.h"
#include "sequoia-engine/Core/Logging.h"

namespace sequoia {

namespace render {

RenderServer::RenderServer() { ressourceCtx_.Join = false; }

RenderServer::~RenderServer() {
  ressourceCtx_.Mutex.lock();
  ressourceCtx_.Join = true;
  ressourceCtx_.TasksAvailable.notify_all();
  ressourceCtx_.Mutex.unlock();

  ressourceCtx_.Thread.join();
}

void RenderServer::initRessourceThread(std::function<void()> initFunction) {
  // Log::info("Initializing ressource thread {} ... ", core::LoggerListener::getThreadID());

  ressourceCtx_.Thread = std::thread([this, &initFunction]() {
    // Initialize the thread
    initFunction();

    std::shared_ptr<Task> curTask = nullptr;
    while(!ressourceCtx_.Join) {

      // Run as many tasks as possible
      while(ressourceCtx_.Queue.try_pop(curTask)) {
        curTask->run();
        curTask.reset();
      }

      // Go to sleep if no more tasks are available
      std::unique_lock<std::mutex> lock(ressourceCtx_.Mutex);
      ressourceCtx_.TasksAvailable.wait(
          lock, [this]() -> bool { return !ressourceCtx_.Queue.empty() || ressourceCtx_.Join; });
    }
  });

  // Log::info("Successfully initialized ressource thread {}", core::LoggerListener::getThreadID());
}

} // namespace render

} // namespace sequoia
