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
#include "sequoia/Core/Listenable.h"
#include "sequoia/Render/Viewport.h"
#include <memory>

namespace sequoia {

namespace render {

class DrawCommandList;

/// @brief A canvas which can receive the results of a rendering operation
/// @ingroup render
class SEQUOIA_API RenderTarget {
public:
  /// @brief Discriminator for RTTI (dyn_cast<> et al.)
  enum RenderTargetKind { RK_RenderWindow, RK_GLRenderWindow, RK_RenderWindowLast };

  /// @brief Set the mode of the cursor
  enum CursorModeKind {
    CK_Disabled, ///< This will hide the cursor and lock it to the specified window. It should be
                 ///  constantly re-centered
    CK_Hidden,   ///< Cursor becomes hidden when it is over a window
    CK_Normal    ///< Keep the cursor provided by the operating system
  };

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

  /// @brief Set the mode of the cursor
  virtual void setCursorMode(CursorModeKind mode) = 0;

  /// @brief Center the cursor in the middle of the window
  virtual void centerCursor() = 0;

  /// @brief Is the target rendered?
  bool isActive() const;

  /// @brief Set if the target is rendered
  void setActive(bool active);

  /// @brief Check if a viewport is attached to the target
  bool hasViewport() const;

  /// @brief Get the viewport
  Viewport* getViewport();
  const Viewport* getViewport() const;

  /// @brief Set the viewport and register it as a `RenderTargetListener`
  void setViewport(const std::shared_ptr<Viewport>& viewport);

  /// @brief Set the `DrawCommandList` which will be rendered
  void setDrawCommandList(const std::shared_ptr<DrawCommandList>& list);

  /// @brief Get the `DrawCommandList`
  const std::shared_ptr<DrawCommandList>& getDrawCommandList();

private:
  RenderTargetKind kind_;

  /// Is the target rendered?
  bool active_;

  /// Viewport connecting the target to a camera
  std::shared_ptr<Viewport> viewport_;

  /// List of draw commands
  std::shared_ptr<DrawCommandList> list_;
};

} // namespace render

} // namespace sequoia

#endif
