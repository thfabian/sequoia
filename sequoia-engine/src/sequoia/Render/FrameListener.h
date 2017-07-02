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

#ifndef SEQUOIA_RENDER_FRAMELISTENER_H
#define SEQUOIA_RENDER_FRAMELISTENER_H

#include "sequoia/Core/Export.h"
#include <cstddef>

namespace sequoia {

namespace render {

class RenderTarget;

/// @brief Listen to frame events
/// @ingroup render
class SEQUOIA_API FrameListener {
public:
  /// @brief Begin of the rendering a frame of `target`
  virtual void frameListenerRenderingBegin(RenderTarget* target) = 0;

  /// @brief End of the rendering of the previous frame of `target`
  virtual void frameListenerRenderingEnd(RenderTarget* target) = 0;
};

} // namespace render

} // namespace sequoia

#endif
