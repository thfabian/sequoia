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

#ifndef SEQUOIA_RENDER_RENDERTARGET_H
#define SEQUOIA_RENDER_RENDERTARGET_H

#include "sequoia/Core/Export.h"
#include "sequoia/Render/RenderFwd.h"
#include "sequoia/Render/Viewport.h"
#include <memory>

namespace sequoia {

namespace render {

/// @brief A canvas which can receive the results of a rendering operation
/// @ingroup render
class SEQUOIA_API RenderTarget {
public:
  /// @brief Discriminator for RTTI (dyn_cast<> et al.)
  enum RenderTargetKind { RK_RenderWindow, RK_GLRenderWindow, RK_RenderWindowLast };

  RenderTarget(RenderTargetKind kind);
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

  /// @brief Get the FrameBufferobeject of the target (if any)
  /// @see RenderSystem::renderOneFrame()
  std::shared_ptr<FrameBufferObject>& getFrameBufferObject() { return fbo_; }

  /// @brief Set the FrameBufferobeject of the target
  void setFrameBufferObject(const std::shared_ptr<FrameBufferObject>& fbo) { fbo_ = fbo; }

  /// @brief Check if a frame buffer has been attached to the RenderTarget
  bool hasFrameBufferObject() const { return fbo_ != nullptr; }

  /// @brief Set the `DrawCommandList` which will be rendered
  void setDrawCommandList(const std::shared_ptr<DrawCommandList>& list) { list_ = list; }

  /// @brief Get the `DrawCommandList`
  const std::shared_ptr<DrawCommandList>& getDrawCommandList() const { return list_; }

private:
  /// RTTI discriminator
  RenderTargetKind kind_;

  /// Viewport connecting the target to a camera
  std::shared_ptr<Viewport> viewport_;

  /// List of draw commands
  std::shared_ptr<DrawCommandList> list_;

  /// Attached FBO of the target (if any)
  std::shared_ptr<FrameBufferObject> fbo_;
};

} // namespace render

} // namespace sequoia

#endif
