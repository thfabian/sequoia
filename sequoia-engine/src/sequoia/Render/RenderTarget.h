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

#ifndef SEQUOIA_RENDER_RENDERTARGET_H
#define SEQUOIA_RENDER_RENDERTARGET_H

#include "sequoia/Core/Listenable.h"
#include "sequoia/Render/Export.h"
#include "sequoia/Render/Viewport.h"
#include <memory>

namespace sequoia {

namespace render {

/// @brief A canvas which can receive the results of a rendering operation.
/// @ingroup render
class SEQUOIA_RENDER_API RenderTarget {
public:
  /// @brief Discriminator for RTTI (dyn_cast<> et al.)
  enum RenderTargetKind { RK_RenderWindow, RK_GLRenderWindow, RK_RenderWindowLast };

  RenderTarget(RenderTargetKind kind);
  virtual ~RenderTarget() {}

  /// @brief Get kind of RenderTarget (used by RTTI dyn_cast<> et al.)
  RenderTargetKind getKind() const { return kind_; }

  /// @brief Initialize the target
  ///
  /// This may include context creation and binding.
  virtual void init() = 0;

  /// @brief Swaps the frame buffers to display the next frame
  virtual void swapBuffers() = 0;

  /// @brief Tells the target to update it's contents
  virtual void update() = 0;

  /// @brief Is the target rendered?
  bool isActive() const;

  /// @brief Set if the target is rendered
  void setActive(bool active);

  /// @brief Get the viewport
  Viewport* getViewport();
  const Viewport* getViewport() const;

  /// @brief Set the viewport and register it as a `RenderTargetListener`
  void setViewport(const std::shared_ptr<Viewport>& viewport);

protected:
  RenderTargetKind kind_;

  /// Is the target rendered?
  bool active_;

  /// Viewport connecting the target to a camera
  std::shared_ptr<Viewport> viewport_;
};

} // namespace render

} // namespace sequoia

#endif
