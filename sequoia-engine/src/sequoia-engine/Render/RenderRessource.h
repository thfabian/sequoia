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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERRESSOURCE_H
#define SEQUOIA_ENGINE_RENDER_RENDERRESSOURCE_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Core/Mutex.h"
#include "sequoia-engine/Core/NonCopyable.h"
#include "sequoia-engine/Render/RenderSystemObject.h"
#include <atomic>
#include <exception>

namespace sequoia {

namespace render {

/// @brief Ressource used by the RenderSystem such as Textures, Shaders and Programs
///
/// @note All render ressources have a single invariant: *Once a Ressource is made valid, it remains
/// valid throughout the lifetime of any object which references it*.
///
/// @ingroup render
class SEQUOIA_API RenderRessource : public RenderSystemObject, public NonCopyable {

  /// Is the ressource is valid? - Once a ressource has been declared valid, it remains valid
  /// throughout the lifetime of the object
  std::atomic<bool> valid_;

  /// Mutex used to modify the state of the object
  Mutex mutex_;

  /// Caught exception (if any) during the construction of the object
  std::exception_ptr exception_;

public:
  RenderRessource(RenderSystemKind renderSystemKind)
      : RenderSystemObject(renderSystemKind), valid_(false), exception_(nullptr) {}
  virtual ~RenderRessource() {}

  /// @brief Check if the ressource is valid
  ///
  /// Once a ressource has been declared valid, it remains valid throughout the lifetime of
  /// the object. It is safe to call this function from any thread.
  ///
  /// @remark Thread-safe
  inline bool isValid() const noexcept { return valid_.load(); }

  /// @brief Make the ressource valid
  ///
  /// This converts the ressource into a valid state s.t `isValid() == true`. Any exceptions caught
  /// during the execution are stored and can be rethrown via `rethrowException()`.
  ///
  /// @remark Thread-safe
  void makeValid();

  /// @brief Rethrow any stored exception
  ///
  /// It no exception was captured, this function does nothing.
  ///
  /// @remark Thread-safe
  void rethrowException();

  /// @brief Check if an exception was captured
  inline bool hasException() const { return exception_ != nullptr; }

protected:
  /// @brief Make the ressource ready
  virtual void makeValidImpl() = 0;
};

} // namespace render

} // namespace sequoia

#endif
