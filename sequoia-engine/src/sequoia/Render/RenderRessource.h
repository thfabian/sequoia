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

#ifndef SEQUOIA_RENDER_RENDERRESSOURCE_H
#define SEQUOIA_RENDER_RENDERRESSOURCE_H

#include "sequoia/Core/Export.h"
#include "sequoia/Core/Mutex.h"
#include "sequoia/Core/NonCopyable.h"
#include "sequoia/Render/RenderSystemObject.h"
#include <atomic>

namespace sequoia {

namespace render {

/// @brief Ressource used by the RenderSystem such as Textures, Shaders and Programs
///
/// @ingroup render
class SEQUOIA_API RenderRessource : public RenderSystemObject, public NonCopyable {

  /// Is the ressource is valid? - Once a ressource has been declared valid, it remains valid
  /// throughout the lifetime of the object
  std::atomic<bool> valid_;

  /// Mutex used to modify the state of the object i.e the variable `valid_`.
  Mutex mutex_;

public:
  RenderRessource() : valid_(false) {}

  /// @brief Check if the ressource is valid
  ///
  /// Once a ressource has been declared valid, it remains valid throughout the lifetime of
  /// the object. It is safe to call this function from any thread.
  ///
  /// @remark Thread-safe
  inline bool isValid() const noexcept { return valid_.load(); }

  /// @brief Make the ressource valid
  ///
  /// This converts the ressource into a valid state s.t `isValid() == true`. This function should
  /// generally only be called by render-system threads.
  ///
  /// @remark Thread-safe
  inline void makeValid() {
    if(isValid())
      return;

    SEQUOIA_LOCK(mutex_);

    // Prevent the ressource from being initialized multiple times
    if(isValid())
      return;

    // Note that the mutex is released if an exception is encounered
    makeValidImpl();

    valid_.store(true);
    SEQUOIA_UNLOCK(mutex_);
  }

protected:
  /// @brief Make the ressource ready
  virtual void makeValidImpl() = 0;
};
