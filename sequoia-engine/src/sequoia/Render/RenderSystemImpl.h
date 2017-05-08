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

#ifndef SEQUOIA_RENDER_RENDERSYSTEMIMPL_H
#define SEQUOIA_RENDER_RENDERSYSTEMIMPL_H

#include "sequoia/Render/Export.h"
#include "sequoia/Render/RenderFwd.h"
#include <memory>
#include <string>

namespace sequoia {

namespace render {

/// @brief Interface of the various render-system implementations
/// @ingroup render
class SEQUOIA_RENDER_API RenderSystemImpl {
public:
  /// @brief Terminate the render-system
  virtual ~RenderSystemImpl() {}

  /// @brief Create a new RenderWindow
  ///
  /// @param title    The initial, UTF-8 encoded window title
  /// @returns the created window
  virtual RenderWindow* createWindow(const std::string& title) = 0;

  /// @brief Processes events that are in the event queue
  virtual void pollEvents() = 0;

  /// @brief Render one frame into the target of each active registered RenderTarget
  virtual void renderOneFrame() = 0;

  /// @brief Swap the buffers and display the next frame for each active registered RenderTarget
  virtual void swapBuffers() = 0;
};

} // namespace render

} // namespace sequoia

#endif
