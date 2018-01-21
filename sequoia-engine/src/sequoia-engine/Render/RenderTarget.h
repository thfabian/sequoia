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

#ifndef SEQUOIA_ENGINE_RENDER_RENDERTARGET_H
#define SEQUOIA_ENGINE_RENDER_RENDERTARGET_H

#include "sequoia-engine/Core/Export.h"
#include "sequoia-engine/Render/Viewport.h"
#include <memory>

namespace sequoia {

namespace render {

/// @brief A canvas which can receive the results of a rendering operation
/// @ingroup render
class SEQUOIA_API RenderTarget {
public:
  /// @brief Discriminator for RTTI (dyn_cast<> et al.)
  enum RenderTargetKind {
    RK_RenderWindow,
    RK_GLRenderWindow,
    RK_NullRenderWindow,
    RK_D3D12RenderWindow,
    RK_RenderWindowLast,

    // RK_RenderTexture
  };

  /// @brief Initialize the target
  RenderTarget(RenderTargetKind kind);

  /// @brief Virtual destructor
  virtual ~RenderTarget() {}

  /// @brief Get kind of RenderTarget (used by RTTI dyn_cast<> et al.)
  RenderTargetKind getKind() const { return kind_; }

  /// @brief Check if a viewport is attached to the target
  bool hasViewport() const { return viewport_ != nullptr; }

  /// @brief Get the viewport
  Viewport* getViewport();
  const Viewport* getViewport() const;

  /// @brief Set the viewport and register it as a `RenderTargetListener`
  void setViewport(const std::shared_ptr<Viewport>& viewport) { viewport_ = viewport; }

  /// @brief Convert to string
  std::string toString() const;

private:
  /// RTTI discriminator
  RenderTargetKind kind_;

  /// Viewport connecting the target to a camera
  std::shared_ptr<Viewport> viewport_;
};

} // namespace render

} // namespace sequoia

#endif
