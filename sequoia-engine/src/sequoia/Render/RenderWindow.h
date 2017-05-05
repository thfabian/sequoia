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

#ifndef SEQUOIA_RENDER_RENDERWINDOW_H
#define SEQUOIA_RENDER_RENDERWINDOW_H

#include "sequoia/Render/Export.h"

namespace sequoia {

namespace render {

/// @brief Abstract base class of all RenderWindows
///
/// RenderWindows are created by the RenderSystem.
/// @ingroup render
class SEQUOIA_RENDER_API RenderWindow {
public:
  virtual ~RenderWindow() {}

  /// @brief Poll if the window is open
  virtual bool isOpen() = 0;
};

} // namespace render

} // namespace sequoia

#endif
